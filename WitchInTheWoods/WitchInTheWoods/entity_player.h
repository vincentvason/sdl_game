#pragma once
#include "load.h"
#include "entity.h"
#include "entity_bullet.h"
#include "entity_tile.h"
#include "entity_enemy.h"

class PlayerEntity : public Entity
{
public:
	PlayerEntity(LTexture* sprite, int x = 0, int y = 0);
	void init();
	void update(TileGroup tileGroup);
	void move(enum Facing facing);
	void shoot();
	SDL_Rect getPosition();
	void setPosition(int x = 0, int y = 0);
	bool getActive() { return isActive; };
	void setActive(bool active) { isActive = active; };
	bool getInvisible();
	void setInvisible(bool active);
	Entity::Facing getFacing();
	bool getDeadStatus() { return isDead; };
	void playDeadAnimation();


protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;

	enum Facing mAnimFacing = FACING_DOWN;
	float mAnimWalking = 0;

	int mInvisibleFrame = 0;
	int mDeadFrame = 0;
	bool isDead = false;

	SDL_Rect mCollider;
	LTexture* mSprite;

private:
	const float DEFAULT_VEL = 6;
	const float DEFAULT_STEP_SPEED = 0.5;

	const int DEFAULT_INVISIBLE_FRAME = 200;
	const int DEFAULT_DEAD_FRAME = 300;

	const int WALKING_FRAME = 3;
	const int ATTACK_SPRITE = 3;

	bool snapBorderCollision();
	bool snapStageCollision(std::vector<TileEntity> vTile);
	
};

class PlayerGroup : public Entity
{
public:
	PlayerEntity p1 = PlayerEntity(&gWitch1Texture);
	PlayerEntity p2 = PlayerEntity(&gWitch1Texture);

	void checkEnemyCollision(EnemySpawner enemies);
	void move(int player, Facing dir);
	void update(TileGroup tileGroup);
};
