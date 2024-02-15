#include "EBullet.h"

LTexture gBulletTexture;

EBullet::EBullet()
{
	//Initialize the offsets
	mPosX = 0;
	mPosY = 0;

	//Set collision box dimension
	mCollider.w = SPRITE_WIDTH;
	mCollider.h = SPRITE_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void EBullet::spawnBullet(int x, int y, int vel)
{
	mPosX = x;
	mPosY = y;
	mVelY = -vel;
	isActive = true;
	shiftColliders();
}

void EBullet::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: mVelX -= SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX += SPRITE_VEL; break;
		case SDLK_a: mVelX -= SPRITE_VEL; break;
		case SDLK_d: mVelX += SPRITE_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: mVelX += SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX -= SPRITE_VEL; break;
		case SDLK_a: mVelX += SPRITE_VEL; break;
		case SDLK_d: mVelX -= SPRITE_VEL; break;
		}
	}
}

void EBullet::move()
{
	//Move the dot left or right
	mPosX += mVelX;
	mCollider.x = mPosX;

	//Move the dot up or down
	mPosY += mVelY;
	mCollider.y = mPosY;

	//If the dot collided or went too far up or down
	if ((mPosY < 0) || (mPosY + SPRITE_HEIGHT > SCREEN_HEIGHT))
	{
		//Move back
		isActive = false;
	}
}

void EBullet::render()
{
	gBulletTexture.render(mPosX, mPosY);
}

SDL_Rect& EBullet::getCollider()
{
	return mCollider;
}
