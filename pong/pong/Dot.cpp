#include "Dot.h"

//Scene textures
LTexture gDotTexture;

Dot::Dot(int x, int y)
{
	//Initialize the offsets
	mPosX = x;
	mPosY = y;
	mStartPosX = x;
	mStartPosY = y;

	//Set collision circle size
	mCollider.r = DOT_WIDTH / 2;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	//Move collider relative to the circle
	shiftColliders();
}

void Dot::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		//Adjust the velocity
		if (e.key.keysym.sym == SDLK_d && isP1begin == true)
		{
			mVelX = 5;
			mVelY = rand() % 5;
			isP1begin = false;
			isP2begin = false;
		}
		if (e.key.keysym.sym == SDLK_LEFT && isP2begin == true)
		{
			mVelX = -5;
			mVelY = -1 * (rand() % 5);
			isP1begin = false;
			isP2begin = false;
		}
		if (e.key.keysym.sym == SDLK_p)
		{
			printf("game reset.\n");
			mVelX = 0;
			mVelY = 0;
			isP1begin = true;
			isP2begin = true;
			p1Score = 0;
			p2Score = 0;
		}
	}
}

void Dot::move(SDL_Rect& paddle1, SDL_Rect& paddle2)
{
	//get point and reset when 
	if (mPosX - mCollider.r < 0)
	{
		mPosX = mStartPosX;
		mPosY = mStartPosY;
		mVelX = 0;
		mVelY = 0;
		printf("P2 Goal! Current Score P1 - P2 : %d - %d\n",p1Score,++p2Score);
		printf("P1 press [d] to kick off, or press [p] to reset game.\n");
		isP1begin = true;
	}
	else if (mPosX + mCollider.r > SCREEN_WIDTH)
	{
		mPosX = mStartPosX;
		mPosY = mStartPosY;
		mVelX = 0;
		mVelY = 0;
		printf("P1 Goal! Current Score P1 - P2 : %d - %d\n", ++p1Score, p2Score);
		printf("P2 press [left] to kick off, or press [p] to reset game.\n");
		isP2begin = true;
	}
	
	//If the dot collided to paddle
	if (checkCollision(mCollider, paddle1) || checkCollision(mCollider, paddle2))
	{
		//Reflect
		mVelX = (mVelX * -1);
	}

	//If the dot collided or went too far up or down
	if ((mPosY - mCollider.r < 0) || (mPosY + mCollider.r > SCREEN_HEIGHT) || checkCollision(mCollider, paddle1) || checkCollision(mCollider, paddle2))
	{
		//Reflect
		mVelY = (mVelY * -1);
		mPosY += mVelY;
		shiftColliders();
	}

	//Move the dot left or right
	mPosX += mVelX;
	shiftColliders();

	//Move the dot up or down
	mPosY += mVelY;
	shiftColliders();

	//Speed up when collide to paddle
	if (checkCollision(mCollider, paddle1) || checkCollision(mCollider, paddle2))
	{
		mVelX = (mVelX > 0) ? mVelX + (rand() % 2) : mVelX - (rand() % 2);
		mVelY = (mVelY > 0) ? rand() % mVelX : (rand() % mVelX) * -1;
	}
}

void Dot::render()
{
	if (isP1begin == true && isP2begin == false)
	{
		gDotTexture.setColor(0xFF, 0x00, 0xFF);
	}
	else if (isP1begin == false && isP2begin == true)
	{
		gDotTexture.setColor(0x00, 0xFF, 0xFF);
	}
	else
	{
		gDotTexture.setColor(0xFF, 0xFF, 0xFF);
	}
	
	gDotTexture.render(mPosX - mCollider.r, mPosY - mCollider.r);
}

Circle& Dot::getCollider()
{
	return mCollider;
}

void Dot::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}