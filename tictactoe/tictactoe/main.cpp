/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"
#include "LButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 480;

//Spaces
char tables[10] = "---------";

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Tic-tac-toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButtonSpriteSheetTexture.loadFromFile("img/button.png"))
	{
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else
	{
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 160;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		//Set buttons
		for (int i = 0; i < TOTAL_BUTTONS; ++i)
		{
			gButtons[i].setPosition((i / 3) * BUTTON_WIDTH, (i % 3) * BUTTON_HEIGHT);
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void checkWin()
{
	for (int i = 0; i <= 2; ++i)
	{
		if (tables[(i * 3)] == tables[(i * 3) + 1] && tables[(i * 3) + 1] == tables[(i * 3) + 2] && tables[(i * 3) + 2] == 'o')
		{
			tables[(i * 3)] = 'O';
			tables[(i * 3) + 1] = 'O';
			tables[(i * 3) + 2] = 'O';
			
			for (int i = 0; i < 9; ++i)
			{
				if (tables[i] == '-') tables[i] = 'A';
			}
		}
		else if (tables[(i * 3)] == tables[(i * 3) + 1] && tables[(i * 3) + 1] == tables[(i * 3) + 2] && tables[(i * 3) + 2] == 'x')
		{
			tables[(i * 3)] = 'X';
			tables[(i * 3) + 1] = 'X';
			tables[(i * 3) + 2] = 'X';

			for (int i = 0; i < 9; ++i)
			{
				if (tables[i] == '-') tables[i] = 'A';
			}
		}
		else if (tables[i] == tables[i + 3] && tables[i + 3] == tables[i + 6] && tables[i + 6] == 'o')
		{
			tables[i] = 'O';
			tables[i + 3] = 'O';
			tables[i + 6] = 'O';

			for (int i = 0; i < 9; ++i)
			{
				if (tables[i] == '-') tables[i] = 'A';
			}
		}
		else if (tables[i] == tables[i + 3] && tables[i + 3] == tables[i + 6] && tables[i + 6] == 'x')
		{
			tables[i] = 'X';
			tables[i + 3] = 'X';
			tables[i + 6] = 'X';

			for (int i = 0; i < 9; ++i)
			{
				if (tables[i] == '-') tables[i] = 'A';
			}
		}
	}

	if (tables[0] == tables[4] && tables[4] == tables[8] && tables[8] == 'o')
	{
		tables[0] = 'O';
		tables[4] = 'O';
		tables[8] = 'O';

		for (int i = 0; i < 9; ++i)
		{
			if (tables[i] == '-') tables[i] = 'A';
		}
	}
	else if (tables[0] == tables[4] && tables[4] == tables[8] && tables[8] == 'x')
	{
		tables[0] = 'X';
		tables[4] = 'X';
		tables[8] = 'X';

		for (int i = 0; i < 9; ++i)
		{
			if (tables[i] == '-') tables[i] = 'A';
		}
	}
	else if (tables[2] == tables[4] && tables[4] == tables[6] && tables[6] == 'o')
	{
		tables[2] = 'O';
		tables[4] = 'O';
		tables[6] = 'O';

		for (int i = 0; i < 9; ++i)
		{
			if (tables[i] == '-') tables[i] = 'A';
		}
	}
	else if (tables[2] == tables[4] && tables[4] == tables[6] && tables[6] == 'x')
	{
		tables[2] = 'X';
		tables[4] = 'X';
		tables[6] = 'X';

		for (int i = 0; i < 9; ++i)
		{
			if (tables[i] == '-') tables[i] = 'A';
		}
	}
}

int main(int argc, char* args[])
{
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
			printf("Welcome to Tic-tac-toe!\n");
			printf("Press R to reset game!\n");

			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//User reset game
					if (e.type == SDL_KEYDOWN)
					{
						if (e.key.keysym.sym == SDLK_r)
						{
							for (int i = 0; i < 9; i++)
							{
								tables[i] = '-';
							}
						}
					}

					//Handle button events
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[i].handleEvent(&e, tables, i);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].render();
				}

				checkWin();

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}