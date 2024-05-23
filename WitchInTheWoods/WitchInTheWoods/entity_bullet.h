#pragma once
#include "entity.h"

class Bullet : public Entity
{
public:
	Bullet(LTexture* sprite);
	void active(int x = 0, int y = 0, enum Facing dir = FACING_DOWN);
	void update();
	bool getActive() { return isActive; };

protected:
	bool isActive = false;
	int mVelX = 0, mVelY = 0;

	float mAnimFrame = 0;
	int mSpriteAngle = 0;

	SDL_Rect mCollider = {0, 0, 0, 0};
	LTexture* mSprite = nullptr;

private:
	const float DEFAULT_VEL = 16;
};