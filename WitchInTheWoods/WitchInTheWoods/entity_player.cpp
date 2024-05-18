#include "entity_player.h"

Player::Player(LTexture* sprite, int locX, int locY)
{
	isActive = true;
	mLocX = locX;
	mLocY = locY;
	mSprite = sprite;

	mCollider.x = LocXToPosX(mLocX);
	mCollider.y = LocYToPosY(mLocY);
	mCollider.w = 32;
	mCollider.h = 32;
}

void Player::handleEvent(SDL_Event* e)
{
	if (e->key.state == SDL_PRESSED)
	{
		//Adjust the velocity
		switch (e->key.keysym.sym)
		{
		case SDLK_w: move(FACING_UP); break;
		case SDLK_s: move(FACING_DOWN); break;
		case SDLK_a: move(FACING_LEFT); break;
		case SDLK_d: move(FACING_RIGHT); break;
		}
	}
}

void Player::update()
{
	mLocX = (mLocX < DEFAULT_VEL && mVelX < 0) ? -mVelX : mLocX + mVelX;
	mLocX = (mLocX > (STAGE_SIZE_X - 1 - DEFAULT_VEL) && mVelX > 0) ? STAGE_SIZE_X - 1 : mLocX + mVelX;
	mLocY = (mLocY < DEFAULT_VEL && mVelY < 0) ? -mVelY : mLocY + mVelY;
	mLocY = (mLocY > (STAGE_SIZE_Y - 1 - DEFAULT_VEL) && mVelY > 0) ? STAGE_SIZE_Y - 1 : mLocY + mVelY;

	mCollider.x = LocXToPosX(mLocX);
	mCollider.y = LocYToPosY(mLocY);

	printf("%f, %f / %d, %d\n", mLocX, mLocY, mCollider.x, mCollider.y);

	SDL_Rect clip = { 32 * (mAnimFacing + (int)mAnimWalking), 0, 32, 32 };
	mSprite->render(mCollider.x, mCollider.y, &clip);

	mVelX = 0;
	mVelY = 0;
}

void Player::move(enum Facing facing)
{
	switch (facing)
	{
	case FACING_UP:
		mAnimFacing = FACING_UP;
		mVelY = -DEFAULT_VEL;
		break;
	case FACING_DOWN:
		mAnimFacing = FACING_DOWN;
		mVelY = DEFAULT_VEL;
		break;
	case FACING_LEFT:
		mAnimFacing = FACING_LEFT;
		mVelX = -DEFAULT_VEL;
		break;
	case FACING_RIGHT:
		mAnimFacing = FACING_RIGHT;
		mVelX = DEFAULT_VEL;
		break;
	}

	if (mVelX != 0 || mVelY != 0)
	{
		mAnimWalking += DEFAULT_STEP_SPEED;
		if (mAnimWalking >= 3) mAnimWalking -= 3;
	}
	else
	{
		mAnimWalking = 0;
	}
}