#include "EEnemy.h"
#include "game.h"

LTexture gEnemyTexture;

EEnemy::EEnemy()
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

	mAngle = 0;
	updateSprite();
	isActive = false;
}

void EEnemy::place(int x, int y)
{
	mPosX = x;
	mPosY = y;
	shiftColliders();
	isActive = true;
}

void EEnemy::updateSprite()
{
	mSprite.w = 32;
	mSprite.h = 32;

	//-90..0..90
	if (mAngle > 90) mAngle = 90;
	if (mAngle < -90) mAngle = 90;

	if (mAngle < 0) mSpriteFlip = SDL_FLIP_VERTICAL;
	else mSpriteFlip = SDL_FLIP_NONE;

	if (mAngle != 0)
	{
		mSprite.x = 32 * (2 + (abs(mAngle) % 15));
		mSprite.y = 0;
	}
	else
	{
		mSprite.x = 32 * (time(0) % 2);
		mSprite.y = 0;
	}
}

void EEnemy::move()
{
	//Move the dot left or right
	if (mPosX <= 0 && mVelX < 0)
	{
		mPosX = 0;
	}
	else if (mPosX + SPRITE_WIDTH >= SCREEN_WIDTH && mVelX > 0)
	{
		mPosX = SCREEN_WIDTH - SPRITE_WIDTH;
	}
	else
	{
		mPosX += mVelX;
		mCollider.x = mPosX;
	}

}

bool EEnemy::checkAllCollisions(LEntity* entity, int size)
{
	for (int i = 0; i < size; i++)
	{
		//printf("%d,%d v %d,%d\n", mCollider.x, mCollider.y, entity[i].getCollider().x, entity[i].getCollider().y);
		if (entity[i].isActive == true && checkCollision(mCollider, entity[i].getCollider()) == true)
		{
			isActive = false;
			entity[i].isActive = false;
			mScore += 10;
			return true;
		}
	}
	return false;
}

void EEnemy::shoot(EBullet& bullet)
{
	//TBC
}

void EEnemy::render()
{
	gEnemyTexture.render(mPosX, mPosY, &mSprite, 0, 0, mSpriteFlip);
}

void EEnemy::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}