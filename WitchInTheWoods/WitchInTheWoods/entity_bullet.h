#pragma once
#include "entity.h"
#include "entity_stage.h"
#include <vector>

class BulletEntity : public Entity
{
public:
	BulletEntity() {};
	BulletEntity(LTexture* sprite);
	SDL_Rect getPosition();
	void active(int x = 0, int y = 0, enum Facing dir = FACING_DOWN, enum Owner owner = FREE);
	void update(StageLoader &stages);
	bool getActive() { return isActive; };

protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;
	enum Owner mOwner = FREE;

	float mAnimFrame = 0;
	int mSpriteAngle = 0;
	enum Facing mSpriteFacing = FACING_DOWN;

	SDL_Rect mCollider = {0, 0, 0, 0};
	LTexture* mSprite = nullptr;

private:
	const float DEFAULT_VEL = 16;
	void checkStageCollision(std::vector<StageEntity> &vTile);
};

class BulletGroup : public Entity
{
public:
	BulletGroup(int bulletSlot = 4);
	std::vector<BulletEntity> getBulletVector();

	void active(SDL_Rect playerPosition, enum Facing dir = FACING_DOWN, enum Owner owner = FREE);
	void update(StageLoader &stages);
	bool checkCollisionToAll(SDL_Rect otherCollider);

private:
	int pBulletSlot = 4;
	int pBulletFreeIndex = 0;
	std::vector<BulletEntity> vBullet = std::vector<BulletEntity>(pBulletSlot, BulletEntity(&gBulletTexture));

	int updateBulletFreeIndex();
};