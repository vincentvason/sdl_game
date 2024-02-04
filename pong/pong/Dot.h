#pragma once
#include "LTexture.h"
#include <stdlib.h>

//Scene textures
extern LTexture gDotTexture;

//The dot that will move around on the screen
class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 1;

	//Initializes the variables
	Dot(int x, int y);

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and checks collision
	void move(SDL_Rect& paddle1, SDL_Rect& paddle2);

	//Shows the dot on the screen
	void render();

	//Gets collision circle
	Circle& getCollider();

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;
	int mStartPosX, mStartPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Dot's collision circle
	Circle mCollider;

	//Moves the collision circle relative to the dot's offset
	void shiftColliders();

	//Bool check for kick-off
	bool isP1begin = true, isP2begin = true;

	//Score
	int p1Score = 0, p2Score = 0;
};
