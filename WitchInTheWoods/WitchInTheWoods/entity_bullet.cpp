#include "entity_bullet.h"

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
	mCollider.w = 8;
	mCollider.h = 32;

	switch (dir)
	{
	case FACING_DOWN: mVelX = 0; mVelY = 8; mSpriteAngle = 180; break;
	case FACING_LEFT: mVelX = -8; mVelY = 0; mSpriteAngle = 270; break;
	case FACING_UP: mVelX = 0; mVelY = -8; mSpriteAngle = 0; break;
	case FACING_RIGHT: mVelX = 8; mVelY = 0; mSpriteAngle = 90; break;
	}

	mOwner = owner;
}

void BulletEntity::update()
{
	//Bullet Animation
	if (mAnimFrame < 3) mAnimFrame++;
	SDL_Rect clip = { 8 * mAnimFrame, 0, 8, 32 };

	//Move Bullet
	mCollider.x += mVelX;
	mCollider.y += mVelY;

	if (isActive == true)
	{
		printf("%d, %d, %s\n", mCollider.x, mCollider.y, mSprite->getFilePath().c_str());
		mSprite->render(mCollider.x, mCollider.y, &clip, mSpriteAngle);
	}

	//Check if collision
	if ((mVelX < 0 && mCollider.x <= STAGE_X_BEGIN + 8)|| (mVelY < 0 && mCollider.y < STAGE_Y_BEGIN) || (mVelX > 0 && mCollider.x > STAGE_X_END + 8) || (mVelY > 0 && mCollider.y > STAGE_Y_END))
	{
		isActive = false;
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

void BulletGroup::update()
{
	for (int i = 0; i < pBulletSlot; i++)
	{
		vBullet[i].update();
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