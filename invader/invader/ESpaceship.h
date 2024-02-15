#pragma once
#include "LEntity.h"
#include "EBullet.h"

extern LTexture gSpaceshipTexture;

class ESpaceship : public LEntity
{
public:
	//The dimensions of the dot
	static const int SPRITE_WIDTH = 32;
	static const int SPRITE_HEIGHT = 32;

	//Maximum axis velocity of the dot
	static const int SPRITE_VEL = 5;

	//Initializes the variables
	ESpaceship();

	//Takes key presses and adjusts the spaceship's velocity
	void handleEvent(SDL_Event& e, EBullet& bullet);

	//Moves the spaceship and checks collision
	void move(LEntity* entity, int size);

	//Shoot bullet
	void shoot(EBullet& bullet);

	//Shows the dot on the screen
	void render();

	//Show on screen
	bool isActive = true;

	//True if is collide
	bool isCollide = false;

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//ESpaceship's collision box
	SDL_Rect mCollider;
}; 
