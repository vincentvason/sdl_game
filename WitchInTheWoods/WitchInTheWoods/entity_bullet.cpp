#include "entity_bullet.h"

Bullet::Bullet(LTexture* sprite)
{
	isActive = false;
	mSprite = sprite;
}

void Bullet::active(int x, int y, enum Facing dir)
{
	isActive = true;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 8;
	mCollider.h = 32;

	switch (dir)
	{
	case FACING_DOWN: mVelY = 8; mSpriteAngle = 180; break;
	case FACING_LEFT: mVelX = -8; mSpriteAngle = 270; break;
	case FACING_UP: mVelY = -8; mSpriteAngle = 0; break;
	case FACING_RIGHT: mVelX = 8; mSpriteAngle = 90; break;
	}
}

void Bullet::update()
{
	//Bullet Animation
	if (mAnimFrame < 3) mAnimFrame++;
	SDL_Rect clip = { 8 * mAnimFrame, 0, 8, 32 };

	//Move Bullet
	mCollider.x += mVelX;
	mCollider.y += mVelY;

	//Check if collision
	if (mCollider.x < STAGE_X_BEGIN || mCollider.y < STAGE_Y_BEGIN || mCollider.y > STAGE_Y_END || mCollider.y > STAGE_Y_END)
	{
		isActive = false;
	}
	
	if(isActive == true)
	{
		printf("%d, %d, %s\n", mCollider.x, mCollider.y, mSprite->getFilePath().c_str());
		mSprite->render(mCollider.x, mCollider.y, &clip, mSpriteAngle);
	}
}