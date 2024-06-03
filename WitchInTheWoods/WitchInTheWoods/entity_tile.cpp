#include "entity_tile.h"

TileEntity::TileEntity()
{

}

TileEntity::TileEntity(LTexture* sprite, int x, int y, bool passable)
{
	isActive = true;
	isPassable = passable;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

void TileEntity::init(std::string terrainType, int x, int y,int i)
{
	if (terrainType == "E")
	{
		isActive = true;
		mSprite = &gEdificeTexture;
		isPassable = false;
		isDestructible = true;

	}
	else if (terrainType == "W")
	{
		isActive = true;
		mSprite = &gWallTexture;
		isPassable = false;
		isDestructible = false;
	}
	else if (terrainType == "T")
	{
		isActive = true;
		mSprite = &gTreeTexture;
		isPassable = true;
		isDestructible = false;
	}

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;

	index = i;
}

void TileEntity::damage(enum Facing dir)
{
	switch (dir)
	{
	case FACING_RIGHT: mCollider.x += 8; mClip.x += 8;
	case FACING_LEFT: mCollider.w -= 8; mClip.w -= 8; break;
	case FACING_DOWN: mCollider.y += 8; mClip.y += 8;
	case FACING_UP: mCollider.h -= 8; mClip.h -= 8; break;
	}

	if (mCollider.w == 0 || mClip.w == 0 || mCollider.h == 0 || mClip.h == 0)
	{
		isActive = false;
		isPassable = true;
	}
}

void TileEntity::update()
{
	if (isActive == true)
	{
		mSprite->render(mCollider.x, mCollider.y, &mClip);
	}	
}

SDL_Rect TileEntity::getPosition()
{
	return mCollider;
}

bool TileEntity::getPassable()
{
	return isPassable;
}

bool TileEntity::getDestructible()
{
	return isDestructible;
}

TileGroup::TileGroup()
{

}

void TileGroup::update()
{
	for (int i = 0; i < pTileSlot; i++)
	{
		vTile[i].update();
	}
}