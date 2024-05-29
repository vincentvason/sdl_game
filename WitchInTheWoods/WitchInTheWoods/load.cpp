#include "load.h"

SDL_Window* gWindow = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 480; //32*15
const int SCREEN_HEIGHT = 640;  //32*20

//Image list
LTexture gLogoTexture;
LTexture gIconTexture;
LTexture gMonster1Texture;
LTexture gWitch1Texture;
LTexture gBulletTexture;
LTexture gEdificeTexture;
LTexture gWallTexture;
LTexture gTreeTexture;

//Text list
LTexture gTextTexture;
SDL_Color gTextColor_White = { 0xFF, 0xFF, 0xFF };


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled.\n");
		}

		//Create window
		gWindow = SDL_CreateWindow("Witch in the Wood", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created. SDL Error: %s\n", SDL_GetError());
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
					printf("SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

	//Load logo texture
	if (!gLogoTexture.loadFromFile("img/logo.bmp"))
	{
		printf("Failed to load \"img/logo.bmp\"!\n");
		success = false;
	}

	//Load witch1 texture
	if (!gWitch1Texture.loadFromFile("img/witch1.png"))
	{
		printf("Failed to load \"img/witch1.png\"!\n");
		success = false;
	}

	//Load heart texture
	if (!gIconTexture.loadFromFile("img/heart.png"))
	{
		printf("Failed to load \"img/heart.png\"!\n");
		success = false;
	}

	//Load bullet texture
	if (!gBulletTexture.loadFromFile("img/bullet.png"))
	{
		printf("Failed to load \"img/bullet.png\"!\n");
		success = false;
	}

	//Load monster texture
	if (!gMonster1Texture.loadFromFile("img/monster1.png"))
	{
		printf("Failed to load \"img/monster1.png\"!\n");
		success = false;
	}

	//Load edifice texture
	if (!gEdificeTexture.loadFromFile("img/edifice.png"))
	{
		printf("Failed to load \"img/edifice.png\"!\n");
		success = false;
	}

	//Load wall texture
	if (!gWallTexture.loadFromFile("img/wall.png"))
	{
		printf("Failed to load \"img/wall.png\"!\n");
		success = false;
	}

	//Load wall texture
	if (!gTreeTexture.loadFromFile("img/tree.png"))
	{
		printf("Failed to load \"img/tree.png\"!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("font/font.ttf", 16);
	if (gFont == NULL)
	{
		printf("Failed to load \"font/font.ttf\" Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gLogoTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}