#include "game.h"
#include "load.h"
#include "entity.h"

GameScene* InGameScene::handleEvent(Game& game, SDL_Event* e)
{
	if (e->key.state == SDL_PRESSED)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_w: players.p1.move(Entity::FACING_UP); break;
		case SDLK_a: players.p1.move(Entity::FACING_LEFT); break;
		case SDLK_s: players.p1.move(Entity::FACING_DOWN); break;
		case SDLK_d: players.p1.move(Entity::FACING_RIGHT); break;
		}
	}
	
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_g: 
			players.p1.shoot();
			bullets.init(players.p1.getPosition(), players.p1.getFacing(), Entity::PLAYER_1);
			break;
		case SDLK_BACKSPACE:
			return &menu;
		case SDLK_SPACE: 
			profile.insertCredit(); 
			break;
		case SDLK_LSHIFT: 
			if (profile.getPlayerIn(0) == false)
			{ 
				if (profile.usedCredit())
				{
					profile.setPlayerIn(0, true);
					break; 
				} 
			}
			else break;
		case SDLK_RSHIFT:
			if (profile.getPlayerIn(1) == false)
			{
				if (profile.usedCredit())
				{
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
	players.p1.setActive(profile.getPlayerIn(0));
	players.p2.setActive(profile.getPlayerIn(1));

	//Load Stage (if it's not loaded)
	if (isStageLoaded == false)
	{
		stage.getStageFromFile(1, tiles, enemies, players);
		isStageLoaded = true;

		for (int i = 0; i < 8; i++)
			enemies.spawnEnemyInOrder(i);
	}

	stage.setAllEnemiesMovement(tiles, enemies, players);
	bullets.checkCollisionToEnemies(enemies);

	//Rendering
	players.p1.update(tiles);
	enemies.update(tiles);
	bullets.update(tiles);
	tiles.update();
}
