#pragma once
#include "LTexture.h"

class LEntity
{
public:
	//The dimensions of the dot
	static const int SPRITE_WIDTH = 32;
	static const int SPRITE_HEIGHT = 32;

	//Maximum axis velocity of the dot
	static const int SPRITE_VEL = 5;

	//Initializes the variables
	LEntity();

	//Gets collision
	SDL_Rect& getCollider();

	//Check collision
	bool checkCollision(SDL_Rect a, SDL_Rect b);

	//Moves the collision circle relative to the dot's offset
	void shiftColliders();

	//Show on screen
	bool isActive = false;

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Spaceship's collision box
	SDL_Rect mCollider;

};