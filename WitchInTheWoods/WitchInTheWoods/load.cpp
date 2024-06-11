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
LTexture gFoeBonusTexture;

//Text list
LTexture gTextTexture;
SDL_Color gTextColor_White = { 0xFF, 0xFF, 0xFF };
SDL_Color gTextColor_Red = { 0xFF, 0x00, 0x00 };

//Sound list
Mix_Chunk* gShootSound;
Mix_Chunk* gDieSound;
Mix_Chunk* gStartSound;
Mix_Chunk* gCreditSound;
Mix_Chunk* gDamageSound;


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

				//Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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

	//Load tree texture
	if (!gTreeTexture.loadFromFile("img/tree.png"))
	{
		printf("Failed to load \"img/tree.png\"!\n");
		success = false;
	}

	//Load foe-bonus texture
	if (!gFoeBonusTexture.loadFromFile("img/foe-bonus.png"))
	{
		printf("Failed to load \"img/foe-bonus.png\"!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("font/font.ttf", 16);
	if (gFont == NULL)
	{
		printf("Failed to load \"font/font.ttf\" Error: %s\n", TTF_GetError());
		success = false;
	}

	//Load music
	gShootSound = Mix_LoadWAV("sound/shoot.wav");
	if (gShootSound == NULL)
	{
		printf("Failed to load  \"sound/shoot.wav\" Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load music
	gDieSound = Mix_LoadWAV("sound/die.wav");
	if (gDieSound == NULL)
	{
		printf("Failed to load  \"sound/die.wav\" Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load music
	gStartSound = Mix_LoadWAV("sound/start.wav");
	if (gStartSound == NULL)
	{
		printf("Failed to load  \"sound/start.wav\" Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load music
	gCreditSound = Mix_LoadWAV("sound/credit.wav");
	if (gCreditSound == NULL)
	{
		printf("Failed to load  \"sound/credit.wav\" Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load music
	gDamageSound = Mix_LoadWAV("sound/damage.wav");
	if (gDamageSound == NULL)
	{
		printf("Failed to load  \"sound/damage.wav\" Error: %s\n", Mix_GetError());
		success = false;
	}


	return success;
}

void close()
{
	//Free loaded images
	gLogoTexture.free();
	gWitch1Texture.free();
	gIconTexture.free();
	gBulletTexture.free();
	gMonster1Texture.free();
	gEdificeTexture.free();
	gWallTexture.free();
	gTreeTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Free the music
	Mix_FreeChunk(gShootSound);
	gShootSound = NULL;
	Mix_FreeChunk(gDieSound);
	gDieSound = NULL;
	Mix_FreeChunk(gStartSound);
	gStartSound = NULL;
	Mix_FreeChunk(gCreditSound);
	gCreditSound = NULL;
	Mix_FreeChunk(gCreditSound);
	gCreditSound = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}