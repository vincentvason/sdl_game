#pragma once
#include "entity.h"
#include "entity_tile.h"
#include "entity_enemy.h"
#include "profile.h"
#include <vector>

class BulletEntity : public Entity
{
public:
	BulletEntity() {};
	BulletEntity(LTexture* sprite);
	SDL_Rect getPosition();
	void init(int x = 0, int y = 0, enum Facing dir = FACING_DOWN, enum Owner owner = OWNER_FREE);
	void update(TileGroup& tileGroup);
	bool getActive() { return isActive; };
	void setActive(bool active);

protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;
	enum Owner mOwner = OWNER_FREE;

	int mLifeSpan = 0;
	float mAnimFrame = 0;
	int mSpriteAngle = 0;
	enum Facing mSpriteFacing = FACING_DOWN;

	SDL_Rect mCollider = {0, 0, 0, 0};
	LTexture* mSprite = nullptr;

private:
	const float DEFAULT_VEL = 8;
	const float DEFAULT_LIFESPAN = 4 * 32 / DEFAULT_VEL;
	void checkStageCollision(std::vector<TileEntity> &vTile);
};

class BulletGroup : public Entity
{
public:
	BulletGroup(int bulletSlot = 4);
	std::vector<BulletEntity> getBulletVector();

	void init(SDL_Rect playerPosition, enum Facing dir = FACING_DOWN, enum Owner owner = OWNER_FREE);
	void update(TileGroup& tileGroup);
	bool checkCollisionToEnemies(EnemySpawner& enemies);

private:
	int pBulletSlot = 4;
	int pBulletFreeIndex = 0;
	std::vector<BulletEntity> vBullet = std::vector<BulletEntity>(pBulletSlot, BulletEntity(&gBulletTexture));

	int updateBulletFreeIndex();
};