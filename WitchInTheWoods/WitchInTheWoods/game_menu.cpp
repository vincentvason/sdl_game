#include "game.h"
#include "load.h"

GameScene* MenuScene::handleEvent(Game& game, SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		//Adjust the velocity
		switch (e->key.keysym.sym)
		{
		case SDLK_SPACE: profile.insertCredit(); break;
		case SDLK_LSHIFT: 
			if (profile.usedCredit()) 
			{ 
				profile.setPlayerIn(Profile::PLAYER_1, true);
				return &ingame; 
			}
		case SDLK_RSHIFT:
			if (profile.usedCredit())
			{
				profile.setPlayerIn(Profile::PLAYER_2, true);
				return &ingame;
			}
		}
	}

	return nullptr;
}

void MenuScene::update(Game& game)
{
	//High-Score
	char string[11];
	gTextTexture.loadFromRenderedText("HIGH SCORE", gTextColor_White);
	gTextTexture.render((SCREEN_WIDTH * 0.5) - (gTextTexture.getWidth() * 0.5), 0);
	sprintf_s(string, 11, "%10d", profile.getHighScore());
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render((SCREEN_WIDTH * 0.5) - (gTextTexture.getWidth() * 0.5), 16);

	//Logo
	gLogoTexture.render((SCREEN_WIDTH * 0.5) - (gLogoTexture.getWidth() * 0.5), (SCREEN_HEIGHT * 0.2) - (gLogoTexture.getHeight() * 0.5));
	
	//Credits
	profile.updateCreditHUD();
}