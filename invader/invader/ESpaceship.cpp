#include "ESpaceship.h"

LTexture gSpaceshipTexture;

ESpaceship::ESpaceship()
{
	//Initialize the offsets
	mPosX = (SCREEN_WIDTH / 2) - (SPRITE_WIDTH / 2);
	mPosY = SCREEN_HEIGHT - (3 * SPRITE_HEIGHT);

	//Set collision box dimension
	mCollider.w = SPRITE_WIDTH;
	mCollider.h = SPRITE_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void ESpaceship::handleEvent(SDL_Event& e, EBullet& bullet)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: mVelX -= SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX += SPRITE_VEL; break;
		case SDLK_a: mVelX -= SPRITE_VEL; break;
		case SDLK_d: mVelX += SPRITE_VEL; break;
		case SDLK_j: shoot(bullet); break;
		case SDLK_SPACE: shoot(bullet); break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT: mVelX += SPRITE_VEL; break;
		case SDLK_RIGHT: mVelX -= SPRITE_VEL; break;
		case SDLK_a: mVelX += SPRITE_VEL; break;
		case SDLK_d: mVelX -= SPRITE_VEL; break;
		}
	}
}

void ESpaceship::move(LEntity* entity, int size)
{
	//Move the dot left or right
	if (mPosX <= 0 && mVelX < 0)
	{
		mPosX = 0;
	}
	else if (mPosX + SPRITE_WIDTH >= SCREEN_WIDTH && mVelX > 0)
	{
		mPosX = SCREEN_WIDTH - SPRITE_WIDTH;
	}
	else
	{
		mPosX += mVelX;
		mCollider.x = mPosX;
	}

}

void ESpaceship::shoot(EBullet& bullet)
{
	bullet.spawnBullet(mPosX + (SPRITE_WIDTH / 2), mPosY, EBullet::SPRITE_VEL);
}

void ESpaceship::render()
{
	gSpaceshipTexture.render(mPosX, mPosY);
}