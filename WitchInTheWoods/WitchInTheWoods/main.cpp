#include "LTexture.h"
#include "load.h"
#include "game.h"

int main(int argc, char* args[])
{
	Game* game = new Game();

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

			//While application is running
			while (!quit)
			{
				//Scene Transition
				game->sceneTransition();

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle Input
					game->handleEvent(&e);
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);
				SDL_RenderClear(gRenderer);

				//Update
				game->update();

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
