#pragma once
#include "game.h"
#include "entity.h"

class Player : public Actor
{
public:
	Player(LTexture* sprite, int locX = 0, int locY = 0);
	void handleEvent(SDL_Event* e);
	void update();

protected:
	bool isActive = false;

	float mLocX = 0, mLocY = 0;
	float mVelX = 0, mVelY = 0;

	int mAnimFacing = FACING_DOWN;
	float mAnimWalking = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;

private:
	const float STAGE_SIZE_X = 14;
	const float STAGE_SIZE_Y = 18;

	const float DEFAULT_VEL = 0.05;
	const float DEFAULT_STEP_SPEED = 0.5;

	enum Facing { 
		FACING_DOWN = 0, 
		FACING_LEFT = 4,
		FACING_UP = 8,
		FACING_RIGHT = 12,
		DYING = 16
	};

	enum Animation {
		STEP_NONE = 0,
		STEP_LEFT = 1,
		STEP_RIGHT = 2,
		ATTACK = 3
	};

	void move(enum Facing facing);
};
