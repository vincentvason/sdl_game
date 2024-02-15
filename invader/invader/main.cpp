//Using SDL, SDL_image, standard IO, and strings
#include "load.h"
#include "game.h"
#include "ESpaceship.h"
#include "EBullet.h"
#include "EEnemy.h"

//Entity
const int BULLET_SLOT = 16;
const int ENEMY_SLOT = 64;

//UI & Games
char uScore[9];
char uWave[9];

EBullet bullet[BULLET_SLOT];
EEnemy enemy[ENEMY_SLOT];

//Rendered texture
LTexture gTextTexture;

int returnInactiveBullet()
{
	for (int i = 0; i < 128; i++)
	{
		if (bullet[i].isActive == false)
		{
			return i;
		}
	}

	return -1;
}

void setEnemies()
{
	//Set the enemies
	for (int i = 0; i < 24; i++)
	{
		enemy[i].place(SCREEN_WIDTH / 2 - (48 * 3) + (48 * (i % 6)), 48 + (48 * (i / 6)));
	}
}

int main(int argc, char* args[])
{
	//Render text
	SDL_Color textColor = { 0xFF, 0xFF, 0xFF };

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The spaceship that will be moving around on the screen
			ESpaceship ship;
			int bulletIndex;

			setEnemies();

			//While application is running
			while (!quit)
			{
				//
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle input for the dot
					
					bulletIndex = returnInactiveBullet();
					ship.handleEvent(e, bullet[bulletIndex]);
				}

				//Move the dot and check collision
				ship.move(enemy, sizeof(enemy));
				for (int i = 0; i < BULLET_SLOT; i++)
				{
					if (bullet[i].isActive == true)
					{
						bullet[i].move();
					}
				}
				for (int i = 0; i < ENEMY_SLOT; i++)
				{
					if (enemy[i].isActive == true)
					{
						enemy[i].checkAllCollisions(bullet, BULLET_SLOT);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render dot
				ship.render();

				for (int i = 0; i < 64; i++)
				{
					if (enemy[i].isActive == true)
					{
						enemy[i].updateSprite();
						enemy[i].render();
					}
				}
				for (int i = 0; i < 128; i++)
				{
					if (bullet[i].isActive == true)
					{
						bullet[i].render();
					}
				}

				gTextTexture.loadFromRenderedText("P1", textColor);
				gTextTexture.render(8, 8);
				
				sprintf_s(uScore, "%8d", mScore);
				gTextTexture.loadFromRenderedText(uScore, textColor);
				gTextTexture.render(8, 24);

				sprintf_s(uWave, "WAVE %3d", mWave);
				gTextTexture.loadFromRenderedText(uWave, textColor);
				gTextTexture.render(472 - 16*8, 8);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

