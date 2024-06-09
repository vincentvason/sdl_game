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

void PlayerEntity::init()
{
	mDeadFrame = 0;
	setInvisible(true);
	isDead = false;
	
}

void PlayerEntity::update(TileGroup tileGroup)
{
	if (snapBorderCollision() == false)
	{
		if (snapStageCollision(tileGroup.vTile) == false)
		{
			mCollider.x += mVelX;
			mCollider.y += mVelY;
		}
	}

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

	if (mDeadFrame > 0 && mDeadFrame < DEFAULT_DEAD_FRAME)
		mDeadFrame++;
	else if (mDeadFrame == DEFAULT_DEAD_FRAME)
		isDead = true;
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

bool PlayerEntity::snapBorderCollision()
{

	if (mCollider.x + mVelX < STAGE_X_BEGIN)
	{
		mCollider.x = STAGE_X_BEGIN;
		return true;
	}
	else if (mCollider.x + mVelX > STAGE_X_END)
	{
		mCollider.x = STAGE_X_END;
		return true;
	}
	else
	{
		mCollider.x += mVelX;
	}

	if (mCollider.y + mVelY < STAGE_Y_BEGIN)
	{
		mCollider.y = STAGE_Y_BEGIN;
		return true;
	}
	else if (mCollider.y + mVelY > STAGE_Y_END)
	{
		mCollider.y = STAGE_Y_END;
		return true;
	}
	else
	{
		mCollider.y += mVelY;
	}

	return false;
}

bool PlayerEntity::snapStageCollision(std::vector<TileEntity> vTile)
{
	float gx = (mCollider.x - STAGE_X_BEGIN) / 32.0;
	float gy = (mCollider.y - STAGE_Y_BEGIN) / 32.0;

	const int SLOT = 12;

	int gCheck[SLOT] = {
		((std::floor(gy - 1) * 14) + std::floor(gx)),
		((std::floor(gy) * 14) + std::floor(gx - 1)),
		((std::floor(gy) * 14) + std::floor(gx)),
		((std::floor(gy - 1) * 14) + std::ceil(gx)),
		((std::floor(gy) * 14) + std::ceil(gx + 1)),
		((std::floor(gy) * 14) + std::ceil(gx)),
		((std::ceil(gy + 1) * 14) + std::floor(gx)),
		((std::ceil(gy) * 14) + std::floor(gx - 1)),
		((std::ceil(gy) * 14) + std::floor(gx)),
		((std::ceil(gy + 1) * 14) + std::ceil(gx)),
		((std::ceil(gy) * 14) + std::ceil(gx + 1)),
		((std::ceil(gy) * 14) + std::ceil(gx))
	};

	SDL_Rect futurePlayerRect;
	futurePlayerRect.x = mCollider.x + mVelX;
	futurePlayerRect.y = mCollider.y + mVelY;
	futurePlayerRect.w = mCollider.w;
	futurePlayerRect.h = mCollider.h;

	for (int i = 0; i < SLOT; i++)
	{
		if (gCheck[i] >= 0 && gCheck[i] < 14 * 18)
		{
			SDL_Rect rectCheck = vTile[gCheck[i]].getPosition();
			
			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
			//SDL_RenderFillRect(gRenderer, &rectCheck);
			if (vTile[gCheck[i]].getPassable() == false && Entity::checkCollision(rectCheck, futurePlayerRect) == true)
			{
				if (mVelX < 0) mCollider.x = rectCheck.x + 32;
				if (mVelX > 0) mCollider.x = rectCheck.x - 32;
				if (mVelY < 0) mCollider.y = rectCheck.y + 32;
				if (mVelY > 0) mCollider.y = rectCheck.y - 32;
				return true;
			}
		}
	}
	return false;
}

void PlayerGroup::checkEnemyCollision(EnemySpawner enemies)
{
	for (int ei = 0; ei < enemies.vEnemy.size(); ei++)
	{
		if (enemies.vEnemy[ei].getActive() == true && enemies.vEnemy[ei].getWaitTime() == 0)
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

void PlayerGroup::update(TileGroup tileGroup)
{
	if (p1.getActive() == true)
		p1.update(tileGroup);
	if (p2.getActive() == true)
		p2.update(tileGroup);

	if (p1.getDeadStatus() == true)
		profile.setPlayerIn(Profile::PLAYER_1, false);
	if (p2.getDeadStatus() == true)
		profile.setPlayerIn(Profile::PLAYER_2, false);
}