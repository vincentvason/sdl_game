#include "game.h"
#include "load.h"
#include "entity.h"

GameScene* InGameScene::handleEvent(Game& game, SDL_Event* e)
{
	
	if (e->key.state == SDL_PRESSED)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_w: players.move(Profile::PLAYER_1, Entity::FACING_UP); break;
		case SDLK_a: players.move(Profile::PLAYER_1, Entity::FACING_LEFT); break;
		case SDLK_s: players.move(Profile::PLAYER_1, Entity::FACING_DOWN); break;
		case SDLK_d: players.move(Profile::PLAYER_1, Entity::FACING_RIGHT); break;
		}
	}
	
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_g: 
			if(profile.getLife(Profile::PLAYER_1) > 0)
			{
				players.p1.shoot();
				bullets.init(players.p1.getPosition(), players.p1.getFacing(), Entity::OWNER_PLAYER_1);
				Mix_PlayChannel(1, gShootSound, 0);
			}
			break;
		case SDLK_BACKSPACE:
			return &menu;
		case SDLK_SPACE: 
			profile.insertCredit(); 
			Mix_PlayChannel(3, gCreditSound, 0);
			break;
		case SDLK_LSHIFT: 
			if (profile.getPlayerIn(Profile::PLAYER_1) == false && profile.getGameOverState() == false)
			{ 
				if (profile.usedCredit())
				{
					players.p1.init();
					profile.setPlayerIn(0, true);
					break; 
				} 
			}
			else break;
		case SDLK_RSHIFT:
			if (profile.getPlayerIn(Profile::PLAYER_2) == false && profile.getGameOverState() == false)
			{
				if (profile.usedCredit())
				{
					players.p2.init();
					profile.setPlayerIn(1, true);
					break;
				}
			}
			else break;
		}
	}

	return nullptr;
}

void InGameScene::update(Game& game)
{
	//Update HUD
	profile.updateCreditHUD();
	profile.updatePlayerHUD();
	profile.updateBottomHUD();
	
	//Stage Background
	SDL_Rect stageBackground = {16, 32, 32*14, 32*18};
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x4d, 0x40, 0xFF);
	SDL_RenderDrawRect(gRenderer, &stageBackground);
	SDL_RenderFillRect(gRenderer, &stageBackground);

	//Set Active
	players.p1.setActive(profile.getPlayerIn(Profile::PLAYER_1));
	players.p2.setActive(profile.getPlayerIn(Profile::PLAYER_2));

	//Load Stage (if it's not loaded)
	if (profile.getStageLoadedStatus() == false)
	{
		stage.getStageFromFile(1, tiles, enemies, players);
		profile.setStageLoadedStatus(true);

		enemies.spawnRandomEnemies(3, 0);
	}


	//Spawn Enemy if they are dead
	if (players.p1.getActive() == true || players.p2.getActive() == true)
	{
		if (enemies.getNumberOfEnemies() > 2)
		{
			enemies.spawnRandomEnemies(1, 300);
		}
		else if (enemies.getNumberOfEnemies() > 0)
		{
			enemies.spawnRandomEnemies(2, 300);
		}
		else
		{
			enemies.spawnRandomEnemies(4, 0);
			profile.showBonusNotification();
			profile.addScore(1000, Profile::PLAYER_1);
			profile.addScore(1000, Profile::PLAYER_2);
		}
		
	}

	stage.setAllEnemiesMovement(tiles, enemies, players);
	bullets.checkCollisionToEnemies(enemies);
	players.checkEnemyCollision(enemies);

	//Rendering
	enemies.update(tiles, enemies);
	players.update(tiles);
	bullets.update(tiles);
	tiles.update();

	//Notification
	if (profile.getPlayerIn(Profile::PLAYER_1) == false && profile.getPlayerIn(Profile::PLAYER_2) == false)
		profile.updateGameOverHUD();

	profile.updatePerfectHUD();

	//Time Increment
	if (time < 100)
	{
		time++;
	}
	else
	{
		time = 0;
		profile.incrementStage();
	}
}

GameScene* InGameScene::sceneTransition(Game& game)
{
	if (profile.getGameOverStatus() == true)
	{
		profile.setGameOverStatus(false);
		profile.setStageLoadedStatus(false);
		profile.init();
		players.p1.init();
		players.p2.init();
		enemies.resetEnemies();
		return &menu;
	}
}
