#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

//Button constants
const int BUTTON_WIDTH = 160;
const int BUTTON_HEIGHT = 160;
const int TOTAL_BUTTONS = 9;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_O = 1,
	BUTTON_SPRITE_MOUSE_OVER_X = 2,
	BUTTON_SPRITE_MOUSE_DOWN_O = 3,
	BUTTON_SPRITE_MOUSE_DOWN_X = 4,
	BUTTON_SPRITE_MOUSE_WIN_O = 5,
	BUTTON_SPRITE_MOUSE_WIN_X = 6,
	BUTTON_SPRITE_MOUSE_LOSE = 7,
	BUTTON_SPRITE_TOTAL = 8
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e, char* table, int index);

	//Shows button sprite
	void render();

private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

//Buttons objects
extern LButton gButtons[TOTAL_BUTTONS];

//Mouse button sprites
extern SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
extern LTexture gButtonSpriteSheetTexture;

