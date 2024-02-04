#include "Paddle.h"

Paddle::Paddle(int x, int y)
{
	//Initialize the offsets
	mPosX = x;
	mPosY = y;

	//Set collision circle size
	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = PADDLE_WIDTH;
	mCollider.h = PADDLE_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

void Paddle::handleEvent(SDL_Event& e, SDL_Keycode up, SDL_Keycode down)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		if (e.key.keysym.sym == up)
		{
			mVelY -= PADDLE_VEL;
		}
		if (e.key.keysym.sym == down)
		{
			mVelY += PADDLE_VEL;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		if (e.key.keysym.sym == up)
		{
			mVelY += PADDLE_VEL;
		}
		if (e.key.keysym.sym == down)
		{
			mVelY -= PADDLE_VEL;
		}
	}
}

void Paddle::move(Circle& dot)
{
	//Move the paddle up or down
	mPosY += mVelY;
	shiftColliders();

	//If the paddle collided or went too far up or down
	if ((mPosY < 0) || (mPosY + mCollider.h > SCREEN_HEIGHT) || checkCollision(dot, mCollider))
	{
		//Move back
		mPosY -= mVelY;
		shiftColliders();
	}
}

void Paddle::render(int player)
{
	//Show the paddle
	if (player == 0)
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xFF, 0xFF);
	}
	else if (player == 1)
	{
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
	}
	else
	{
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	
	SDL_RenderDrawRect(gRenderer, &mCollider);
	SDL_RenderFillRect(gRenderer, &mCollider);
}

SDL_Rect& Paddle::getCollider()
{
	return mCollider;
}

void Paddle::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}