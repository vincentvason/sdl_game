#pragma once
#include "LTexture.h"

//The dot that will move around on the screen
class Paddle
{
public:
	//The dimensions of the dot
	static const int PADDLE_WIDTH = 10;
	static const int PADDLE_HEIGHT = 60;

	//Maximum axis velocity of the dot
	static const int PADDLE_VEL = 10;

	//Initializes the variables
	Paddle(int x, int y);

	//Takes key presses and adjusts the paddle's velocity
	void handleEvent(SDL_Event& e, SDL_Keycode up = SDLK_UP, SDL_Keycode down = SDLK_DOWN);

	//Moves the paddle and checks collision
	void move(Circle& dot);

	//Shows the paddle on the screen
	void render(int player);

	//Gets collision
	SDL_Rect& getCollider();

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Key assigned
	SDL_Keycode mKeyUp, mKeyDown;

	//Paddle's collision circle
	SDL_Rect mCollider;

	//Moves the collision circle relative to the dot's offset
	void shiftColliders();
};

