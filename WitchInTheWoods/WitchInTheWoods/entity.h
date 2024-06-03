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
		FACING_DOWN,
		FACING_LEFT,
		FACING_UP,
		FACING_RIGHT,
		DYING,
		IDLE
	};

	int gridX(int grid) { return STAGE_X_BEGIN + (grid * 32); };
	int gridY(int grid) { return STAGE_Y_BEGIN + (grid * 32); };

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