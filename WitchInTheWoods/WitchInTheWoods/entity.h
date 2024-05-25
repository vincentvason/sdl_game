#pragma once
#include "load.h"

class Entity
{
public:
	virtual void update() {};

	enum Owner {
		FREE,
		PLAYER_1,
		PLAYER_2,
		ENEMY
	};

	enum Facing {
		FACING_DOWN = 0,
		FACING_LEFT = 4,
		FACING_UP = 8,
		FACING_RIGHT = 12,
		DYING = 16
	};

	enum Tag {
		NONE,
		ENEMY_GROUND,
		ENEMY_FLYING,
		ENEMY_BULLET,
		PLAYER,
		PLAYER_BULLET
	};

	bool checkCollision(SDL_Rect a, SDL_Rect b);

protected:
	const int STAGE_X_BEGIN = 16;
	const int STAGE_X_END = STAGE_X_BEGIN + (13 * 32);
	const int STAGE_Y_BEGIN = 32;
	const int STAGE_Y_END = STAGE_Y_BEGIN + (17 * 32);

	bool isActive = false;
	int mVelX = 0, mVelY = 0;

	SDL_Rect mCollider = { 0, 0, 0, 0 };
	LTexture* mSprite = nullptr;
};