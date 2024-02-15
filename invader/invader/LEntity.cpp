#include "LEntity.h"

LEntity::LEntity()
{
	//Initialize the offsets
	mPosX = 0;
    mPosY = 0;

	//Set collision box dimension
	mCollider.w = SPRITE_WIDTH;
	mCollider.h = SPRITE_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
}

SDL_Rect& LEntity::getCollider()
{
	return mCollider;
}

bool LEntity::checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void LEntity::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}