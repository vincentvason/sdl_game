#pragma once
#include "LEntity.h"

extern LTexture gBulletTexture;

class EBullet : public LEntity
{
public:
	//The dimensions of the dot
	static const int SPRITE_WIDTH = 6;
	static const int SPRITE_HEIGHT = 15;

	//Maximum axis velocity of the dot
	static const int SPRITE_VEL = 20;

	//Initializes the variables
	EBullet();

	//Spawn EBullet at the designated location
	void spawnBullet(int x, int y, int vel = 0);

	//Takes key presses and adjusts the spaceship's velocity
	void handleEvent(SDL_Event& e);

	//Moves the spaceship and checks collision
	void move();

	//Shows the dot on the screen
	void render();

	//Gets collision
	SDL_Rect& getCollider();

	//Show on screen
	bool isActive = false;

	//True if is collide
	bool isCollide = false;

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//EBullet's collision box
	SDL_Rect mCollider;

};
