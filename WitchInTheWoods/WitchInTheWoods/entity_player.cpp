#include "entity_player.h"

PlayerEntity::PlayerEntity(LTexture* sprite, int x, int y)
{
	isActive = true;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

SDL_Rect PlayerEntity::getPosition()
{
	return mCollider;
}

Entity::Facing PlayerEntity::getFacing()
{
	return mAnimFacing;
}

void PlayerEntity::update()
{
	checkBorderCollision();
	printf("%d(%f), %d(%f), %s\n", mCollider.x, (mCollider.x - STAGE_X_BEGIN)/ 32.0, mCollider.y, (mCollider.y - STAGE_Y_BEGIN) / 32.0, mSprite->getFilePath().c_str());

	SDL_Rect clip = { 32 * (mAnimFacing + (int)mAnimWalking), 0, 32, 32 };
	mSprite->render(mCollider.x, mCollider.y, &clip);

	mVelX = 0;
	mVelY = 0;
}

void PlayerEntity::move(enum Facing facing)
{
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

void PlayerEntity::shoot()
{
	mAnimWalking = 3;	
}

void PlayerEntity::checkBorderCollision()
{
	if (mCollider.x + mVelX < STAGE_X_BEGIN) mCollider.x = STAGE_X_BEGIN;
	else if (mCollider.x + mVelX > STAGE_X_END) mCollider.x = STAGE_X_END;
	else mCollider.x += mVelX;

	if (mCollider.y + mVelY < STAGE_Y_BEGIN) mCollider.y = STAGE_Y_BEGIN;
	else if (mCollider.y + mVelY > STAGE_Y_END) mCollider.y = STAGE_Y_END;
	else mCollider.y += mVelY;
}

void PlayerEntity::checkStageCollision()
{
	float gridX = (mCollider.x / 32.0) - STAGE_X_BEGIN;
	float gridY = (mCollider.y / 32.0) - STAGE_Y_BEGIN;
}