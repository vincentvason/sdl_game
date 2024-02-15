#pragma once
#pragma once
#include "LEntity.h"
#include "EBullet.h"

extern LTexture gEnemyTexture;

class EEnemy : public LEntity
{
public:
	//The dimensions of the dot
	static const int SPRITE_WIDTH = 32;
	static const int SPRITE_HEIGHT = 32;

	//Maximum axis velocity of the dot
	static const int SPRITE_VEL = 5;

	//Initializes the variables
	EEnemy();

	//Sprite Place
	void place(int x, int y);

	//Sprite Selection
	void updateSprite();

	//Moves the spaceship and checks collision
	void move();
	bool checkAllCollisions(LEntity* entity, int size);

	//Shoot bullet
	void shoot(EBullet& bullet);

	//Shows the dot on the screen
	void render();

	//Gets collision
	SDL_Rect& getCollider();

	//Show on screen
	bool isActive = true;

	//True if is collide
	bool isCollide = false;

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;
	int mAngle;

	//Sprite Successor
	SDL_Rect mSprite;
	SDL_RendererFlip mSpriteFlip = SDL_FLIP_NONE;

	//EEnemy's collision box
	SDL_Rect mCollider;

	void shiftColliders();
};
