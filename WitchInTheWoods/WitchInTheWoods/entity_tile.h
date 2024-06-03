#pragma once
#include "entity.h"
#include <vector>

class TileEntity : public Entity
{
public:
	TileEntity();
	TileEntity(LTexture* sprite, int x = 0, int y = 0, bool passable = false);
	void init(std::string terrainType, int x = 0, int y = 0, int i = -1);
	void damage(enum Facing dir);
	void update();
	SDL_Rect getPosition();
	bool getPassable();
	bool getDestructible();
	
	SDL_Rect mCollider;

protected:

private:
	int index = -1;

	bool isActive = false;
	float mAnimState = 0;
	bool isDestructible = false;
	bool isPassable = true;
	LTexture* mSprite = nullptr;
	
	
	SDL_Rect mClip = {0, 0, 32, 32};
};

class TileGroup : public Entity
{
public:
	TileGroup();
	void update();

	int pTileSlot = 14 * 18;
	std::vector<TileEntity> vTile = std::vector<TileEntity>(pTileSlot, TileEntity());
};