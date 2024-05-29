#include "entity_enemy.h"

EnemyEntity::EnemyEntity(LTexture* sprite, int x, int y)
{
	isActive = true;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

SDL_Rect EnemyEntity::getPosition()
{
	return mCollider;
}

Entity::Facing EnemyEntity::getFacing()
{
	return mAnimFacing;
}

bool EnemyEntity::getActive()
{
	return isActive;
}

void EnemyEntity::setActive(bool active)
{
	isActive = active;
}

void EnemyEntity::update()
{
	if (mCollider.x + mVelX < STAGE_X_BEGIN) mCollider.x = STAGE_X_BEGIN;
	else if (mCollider.x + mVelX > STAGE_X_END) mCollider.x = STAGE_X_END;
	else mCollider.x += mVelX;

	if (mCollider.y + mVelY < STAGE_Y_BEGIN) mCollider.y = STAGE_Y_BEGIN;
	else if (mCollider.y + mVelY > STAGE_Y_END) mCollider.y = STAGE_Y_END;
	else mCollider.y += mVelY;

	SDL_Rect clip = { 32 * (mAnimFacing + (int)mAnimWalking), 0, 32, 32 };
	
	if (isActive == true)
	{
		printf("%d, %d, %s\n", mCollider.x, mCollider.y, mSprite->getFilePath().c_str());
		mSprite->render(mCollider.x, mCollider.y, &clip);
	}
	

	mVelX = 0;
	mVelY = 0;
}

void EnemyEntity::move(SDL_Rect playerPosition)
{
	enum Facing facing = FACING_DOWN;
	switch (facing)
	{
	case FACING_UP:
		mAnimFacing = FACING_UP;
		mVelX = 0;
		mVelY = -DEFAULT_VEL;
		break;
	case FACING_DOWN:
		mAnimFacing = FACING_DOWN;
		mVelX = 0;
		mVelY = DEFAULT_VEL;
		break;
	case FACING_LEFT:
		mAnimFacing = FACING_LEFT;
		mVelX = -DEFAULT_VEL;
		mVelY = 0;
		break;
	case FACING_RIGHT:
		mAnimFacing = FACING_RIGHT;
		mVelX = DEFAULT_VEL;
		mVelY = 0;
		break;
	}

	if (mVelX != 0 || mVelY != 0)
	{
		mAnimWalking += DEFAULT_STEP_SPEED;
		if (mAnimWalking >= 3) mAnimWalking -= 3;
	}
	else
	{
		mAnimWalking = 0;
	}
}

void EnemyEntity::shoot()
{
	mAnimWalking = 3;	
}