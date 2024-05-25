#pragma once
#include "load.h"
#include "entity.h"
#include <vector>

class EnemyEntity : public Entity
{
public:
	EnemyEntity(LTexture* sprite, int x = 0, int y = 0);
	void update();

	void move(enum Facing facing);
	void shoot();
	SDL_Rect getPosition();
	Entity::Facing getFacing();
	void setActive(bool active);
	bool getActive();

protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;

	enum Facing mAnimFacing = FACING_DOWN;
	float mAnimWalking = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;

private:
	const float DEFAULT_VEL = 4;
	const float DEFAULT_STEP_SPEED = 0.5;

	const int WALKING_FRAME = 3;
	const int ATTACK_SPRITE = 3;
};
