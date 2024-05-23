#include "game.h"
#include "load.h"

GameScene* InGameScene::handleEvent(Game& game, SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		//Adjust the velocity
		switch (e->key.keysym.sym)
		{
		case SDLK_SPACE: game.insertCredit(); break;
		case SDLK_LSHIFT: if (game.getPlayerIn(0) == false) { if (game.usedCredit()) { game.setPlayerIn(0, true); break; } }
						  else break;
		case SDLK_RSHIFT: if (game.getPlayerIn(1) == false) { if (game.usedCredit()) { game.setPlayerIn(1, true); break; } }
						  else break;
		}
	}

	return nullptr;
}

void InGameScene::update(Game& game)
{
	//Update HUD
	game.updateCreditHUD();
	updatePlayerHUD(game);
	updateBottomHUD(game);

	
	//Stage Background
	SDL_Rect stage = {16, 32, 32*14, 32*18};
	SDL_SetRenderDrawColor(gRenderer, 0x26, 0x32, 0x38, 0xFF);
	SDL_RenderDrawRect(gRenderer, &stage);
	SDL_RenderFillRect(gRenderer, &stage);

	//Render Player
	pBulletIndex = updateFreeBulletIndex();
	p1.update();
	for(int i = 0; i < 8; i++)
	{
		vBullet[i].update();
	}
}

void InGameScene::updatePlayerHUD(Game& game)
{
	char string[11];
	SDL_Rect rect;

	if (game.getPlayerIn(0) == true)
	{
		sprintf_s(string, 11, "P1 %07d", pScore[0]);
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 0);

		for (int i = 0; i < 5; i++)
		{
			if (pLife[0] > i)
				rect = { 0, 0, 16, 16 };
			else
				rect = { 16, 0, 16, 16 };
			gIconTexture.render(16 * i, 16, &rect);
		}
	}
	else
	{
		sprintf_s(string, 11, " PRESS TO ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 0);
		sprintf_s(string, 11, "   JOIN   ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 16);
	}

	if (game.getPlayerIn(1) == true)
	{
		sprintf_s(string, 11, "P2 %07d", pScore[0]);
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 0);

		for (int i = 0; i < 5; i++)
		{
			if (pLife[0] > i)
				rect = { 0, 0, 16, 16 };
			else
				rect = { 16, 0, 16, 16 };
			gIconTexture.render(320 + (16 * i), 16, &rect);
		}
	}
	else
	{
		sprintf_s(string, 11, " PRESS TO ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 0);
		sprintf_s(string, 11, "   JOIN   ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 16);
	}
}

void InGameScene::updateBottomHUD(Game& game)
{
	char string[11];
	
	sprintf_s(string, 11, "STAGE %03d", pStage);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(0, 608);
	sprintf_s(string, 11, "HI %07d", game.getHighScore());
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(0, 624);
}

int InGameScene::updateFreeBulletIndex()
{
	int i = 0;
	for (Bullet bullet : vBullet)
	{
		if (bullet.getActive() == false)
		{
			return i;
		}
		else
		{
			i++;
		}
	}

	if (i >= pBulletSlot) return 0;
}