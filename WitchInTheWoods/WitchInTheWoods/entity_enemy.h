#pragma once
#include "load.h"
#include "entity.h"
#include "entity_tile.h"
#include <vector>

class EnemyEntity : public Entity
{
public:
	struct EnemyCommand{
		Facing dir;
		int posX;
		int posY;
	};

	EnemyEntity() {};
	EnemyEntity(LTexture* sprite, int x = 0, int y = 0);
	void init(LTexture* sprite, int x = 0, int y = 0);
	void update(TileGroup tiles);

	void move();
	void shoot();
	SDL_Rect getPosition();
	Entity::Facing getFacing();
	void setActive(bool active);
	bool getActive();

	void checkMoveList();
	void updateMoveList();

	void checkBorderCollision();
	void checkStageCollision(std::vector<TileEntity> vTile);
	void addMovement(Entity::Facing dir, int x, int y);
	void clearMovement();

protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;

	enum Facing mMove = IDLE;
	int mDestX = 0, mDestY = 0;

	enum Facing mAnimFacing = FACING_DOWN;
	float mAnimWalking = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;

	std::vector<EnemyCommand> vCommand;

private:
	const float DEFAULT_VEL = 1;
	const float DEFAULT_STEP_SPEED = 0.5;

	const int WALKING_FRAME = 3;
	const int ATTACK_SPRITE = 3;
};

class EnemySpawner : public Entity
{
public:
	EnemySpawner() {};
	void update(TileGroup tiles);
	void insertSpawnPoint(int index);
	void spawnEnemyInOrder(int order);

	int pEnemySlot = 8;
	std::vector<int> vSpawnPosition;
	std::vector<EnemyEntity> vEnemy = std::vector<EnemyEntity>(pEnemySlot, EnemyEntity());	

};