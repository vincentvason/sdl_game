#include "entity_bullet.h"
#include <cmath>

BulletEntity::BulletEntity(LTexture* sprite)
{
	isActive = false;
	mSprite = sprite;
}

SDL_Rect BulletEntity::getPosition()
{
	return mCollider;
}

void BulletEntity::active(int x, int y, enum Facing dir, enum Owner owner)
{
	isActive = true;

	mCollider.x = x;
	mCollider.y = y;
	mSpriteFacing = dir;

	if (dir == FACING_DOWN || dir == FACING_UP)
	{
		mCollider.w = 8;
		mCollider.h = 32;
	}
	else if (dir == FACING_LEFT || dir == FACING_RIGHT)
	{
		mCollider.w = 32;
		mCollider.h = 8;
		mCollider.y += 12;
	}
	

	switch (dir)
	{
	case FACING_DOWN: mVelX = 0; mVelY = DEFAULT_VEL; mSpriteAngle = 180; break;
	case FACING_LEFT: mVelX = -DEFAULT_VEL; mVelY = 0; mSpriteAngle = 270; break;
	case FACING_UP: mVelX = 0; mVelY = -DEFAULT_VEL; mSpriteAngle = 0; break;
	case FACING_RIGHT: mVelX = DEFAULT_VEL; mVelY = 0; mSpriteAngle = 90; break;
	}

	mOwner = owner;
}

void BulletEntity::update(StageLoader &stages)
{
	//Bullet Animation
	if (mAnimFrame < 3) mAnimFrame++;
	SDL_Rect clip = { 8 * mAnimFrame, 0, 8, 32 };

	//Move Bullet
	mCollider.x += mVelX;
	mCollider.y += mVelY;
	

	if (isActive == true)
	{
		//printf("%d, %d, %s\n", mCollider.x, mCollider.y, mSprite->getFilePath().c_str());
		if(mSpriteFacing == FACING_LEFT || mSpriteFacing == FACING_RIGHT)
			mSprite->render(mCollider.x, mCollider.y - 12, &clip, mSpriteAngle);
		else
			mSprite->render(mCollider.x, mCollider.y, &clip, mSpriteAngle);
	}

	//Check if collision
	if ((mVelX < 0 && mCollider.x <= STAGE_X_BEGIN + 8)|| (mVelY < 0 && mCollider.y < STAGE_Y_BEGIN) || (mVelX > 0 && mCollider.x > STAGE_X_END + 8) || (mVelY > 0 && mCollider.y > STAGE_Y_END))
	{
		isActive = false;
	}

	checkStageCollision(stages.vTile);
}

void BulletEntity::checkStageCollision(std::vector<StageEntity> &vTile)
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
			SDL_Rect rectCheck = vTile.at(gCheck[i]).getPosition();

			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
			//SDL_RenderFillRect(gRenderer, &rectCheck);

			if (vTile.at(gCheck[i]).getPassable() == false && Entity::checkCollision(rectCheck, mCollider) == true)
			{
				if (isActive == true && vTile.at(gCheck[i]).getDestructible() == true)
				{
					vTile.at(gCheck[i]).damage(mSpriteFacing);
				}
				isActive = false;
			}
		}
	}
}

BulletGroup::BulletGroup(int bulletSlot)
{
	pBulletSlot = bulletSlot;
}

std::vector<BulletEntity> BulletGroup::getBulletVector()
{
	return vBullet;
}

bool BulletGroup::checkCollisionToAll(SDL_Rect otherCollision)
{
	for (int i = 0; i < pBulletSlot; i++)
	{
		if (Entity::checkCollision(vBullet[i].getPosition(),otherCollision) == true)
		{
			return true;
		}
	}

	return false;
}

void BulletGroup::active(SDL_Rect playerPosition, enum Facing dir, enum Owner owner)
{
	if (dir == FACING_UP || dir == FACING_DOWN)
	{
		vBullet[updateBulletFreeIndex()].active(playerPosition.x + 12, playerPosition.y, dir, owner);
	}
	else
	{
		vBullet[updateBulletFreeIndex()].active(playerPosition.x, playerPosition.y, dir, owner);
	}
}

void BulletGroup::update(StageLoader &stages)
{
	for (int i = 0; i < pBulletSlot; i++)
	{
		vBullet[i].update(stages);
	}
}

int BulletGroup::updateBulletFreeIndex()
{
	int i = 0;
	for (BulletEntity bullet : vBullet)
	{
		if (bullet.getActive() == false)
		{
			return i;
		}
		else
		{
			i++;
		}
	}

	if (i >= pBulletSlot) return 0;
}