#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LTexture.h"

//Window
extern SDL_Window* gWindow;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//Image Bank
extern LTexture gLogoTexture;
extern LTexture gIconTexture;
extern LTexture gWitch1Texture;
extern LTexture gMonster1Texture;
extern LTexture gBulletTexture;
extern LTexture gEdificeTexture;
extern LTexture gWallTexture;
extern LTexture gTreeTexture;
extern LTexture gFoeBonusTexture;

//Text Bank
extern LTexture gTextTexture;
extern SDL_Color gTextColor_White;
extern SDL_Color gTextColor_Red;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();