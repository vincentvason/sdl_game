#include "entity_player.h"
#include <cmath>

PlayerEntity::PlayerEntity(LTexture* sprite, int x, int y)
{
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

void PlayerEntity::setPosition(int x, int y)
{
	mCollider.x = x;
	mCollider.y = y;
}

Entity::Facing PlayerEntity::getFacing()
{
	return mAnimFacing;
}

void PlayerEntity::update(TileGroup tileGroup)
{
	checkBorderCollision();
	checkStageCollision(tileGroup.vTile);
	//printf("%d(%f), %d(%f), %s\n", mCollider.x, (mCollider.x - STAGE_X_BEGIN)/ 32.0, mCollider.y, (mCollider.y - STAGE_Y_BEGIN) / 32.0, mSprite->getFilePath().c_str());

	int anim;
	switch (mAnimFacing)
	{
	case FACING_DOWN:
		anim = 0;
		break;
	case FACING_LEFT:
		anim = 4;
		break;
	case FACING_UP:
		anim = 8;
		break;
	case FACING_RIGHT:
		anim = 12;
		break;
	case DYING:
		anim = 16;
		break;
	}

	SDL_Rect clip = { 32 * (anim + (int)mAnimWalking), 0, 32, 32 };
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

void PlayerEntity::checkStageCollision(std::vector<TileEntity> vTile)
{
	float gx = (mCollider.x - STAGE_X_BEGIN) / 32.0;
	float gy = (mCollider.y - STAGE_Y_BEGIN) / 32.0;

	
	int gCheck[4] = {
		((std::floor(gy) * 14) + std::floor(gx)),
		((std::floor(gy) * 14) + std::ceil(gx)),
		((std::ceil(gy) * 14) + std::floor(gx)),
		((std::ceil(gy) * 14) + std::ceil(gx))
	};

	for (int i = 0; i < 4; i++)
	{
		if (gCheck[i] >= 0 && gCheck[i] < 14 * 18)
		{
			SDL_Rect rectCheck = vTile[gCheck[i]].getPosition();
			
			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
			//SDL_RenderFillRect(gRenderer, &rectCheck);

			if (vTile[gCheck[i]].getPassable() == false && Entity::checkCollision(rectCheck, mCollider) == true)
			{
				mCollider.x -= mVelX;
				mCollider.y -= mVelY;
			}
		}
	}
}