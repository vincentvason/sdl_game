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

void PlayerEntity::update(TileGroup tileGroup)
{
	checkBorderCollision();
	checkStageCollision(tileGroup.vTile);
	//printf("%d(%f), %d(%f), %s\n", mCollider.x, (mCollider.x - STAGE_X_BEGIN)/ 32.0, mCollider.y, (mCollider.y - STAGE_Y_BEGIN) / 32.0, mSprite->getFilePath().c_str());

	int anim;
	int frame;
	SDL_Rect clip;

	if (mDeadFrame > 0)
	{
		anim = 16;
		frame = mDeadFrame / (DEFAULT_DEAD_FRAME / 4);
		clip = { 32 * (anim + frame), 0, 32, 32};
	}
	else
	{
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
		}
		clip = { 32 * (anim + (int)mAnimWalking), 0, 32, 32 };
	}
	
	if(((mInvisibleFrame/10) % 2 == 0) && (mDeadFrame < DEFAULT_DEAD_FRAME))
		mSprite->render(mCollider.x, mCollider.y, &clip);

	mVelX = 0;
	mVelY = 0;

	if (mInvisibleFrame > 0) mInvisibleFrame--;
	if (mDeadFrame > 0 && mDeadFrame < DEFAULT_DEAD_FRAME) mDeadFrame++;
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

SDL_Rect PlayerEntity::getPosition()
{
	return mCollider;
}

void PlayerEntity::setPosition(int x, int y)
{
	mCollider.x = x;
	mCollider.y = y;
}

bool PlayerEntity::getInvisible()
{
	if (mInvisibleFrame > 0)
		return true;
	else
		return false;
}

void PlayerEntity::setInvisible(bool active)
{
	if (active)
		mInvisibleFrame = DEFAULT_INVISIBLE_FRAME;
	else
		mInvisibleFrame = 0;
}

Entity::Facing PlayerEntity::getFacing()
{
	return mAnimFacing;
}

void PlayerEntity::playDeadAnimation()
{
	if (mDeadFrame < DEFAULT_DEAD_FRAME) mDeadFrame++;
}

void PlayerEntity::checkBorderCollision()
{

	mCollider.x += mVelX;
	mCollider.y += mVelY;

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

void PlayerGroup::checkEnemyCollision(EnemySpawner enemies)
{
	for (int ei = 0; ei < enemies.vEnemy.size(); ei++)
	{
		if (enemies.vEnemy[ei].getActive() == true)
		{
			if(p1.getInvisible() == false)
			{
				if (checkCollision(p1.getPosition(), enemies.vEnemy[ei].getPosition()) == true)
				{
					profile.decrementLife(Profile::PLAYER_1);
					if (profile.getLife(Profile::PLAYER_1) > 0)
						p1.setInvisible(true);
					else
						p1.playDeadAnimation();
				}
			}

			if (p2.getInvisible() == false)
			{
				if (checkCollision(p2.getPosition(), enemies.vEnemy[ei].getPosition()) == true)
				{
					profile.decrementLife(Profile::PLAYER_2);
					if (profile.getLife(Profile::PLAYER_2) > 0)
						p2.setInvisible(true);
					else
						p2.playDeadAnimation();
				}
			}
		}
	}
}

void PlayerGroup::move(int player, Facing dir)
{
	if (player == Profile::PLAYER_1 && profile.getLife(Profile::PLAYER_1) > 0)
		p1.move(dir);
	else if (player == Profile::PLAYER_2 && profile.getLife(Profile::PLAYER_2) > 0)
		p2.move(dir);
}