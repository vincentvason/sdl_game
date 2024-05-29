#include "entity_stage.h"
#include <iostream>
#include <fstream>
#include <sstream>

StageEntity::StageEntity()
{

}

StageEntity::StageEntity(LTexture* sprite, int x, int y, bool passable)
{
	isActive = true;
	isPassable = passable;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

void StageEntity::init(std::string terrainType, int x, int y,int i)
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

void StageEntity::damage(enum Facing dir)
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

void StageEntity::update()
{
	if (isActive == true)
	{
		mSprite->render(mCollider.x, mCollider.y, &mClip);
	}	
}

SDL_Rect StageEntity::getPosition()
{
	return mCollider;
}

bool StageEntity::getPassable()
{
	return isPassable;
}

bool StageEntity::getDestructible()
{
	return isDestructible;
}

StageLoader::StageLoader()
{

}

void StageLoader::getStageFromFile(int stage_no)
{
	std::ifstream stageFile("stage/stage_" + std::to_string(stage_no) + ".csv");

	if (!stageFile.is_open() && pLastStage == 0)
	{
		pLastStage = stage_no - 1;
		stageFile.open("stage/stage_" + std::to_string(stage_no % pLastStage) + ".csv");
	}

	if (!stageFile.is_open())
		throw std::runtime_error("Could not open file");

	std::string line;
	
	for(int gy = 0; gy < 18; gy++)
	{
		std::getline(stageFile, line);
		std::stringstream ls(line);
		std::string grid;

		for (int gx = 0; gx < 14; gx++)
		{
			std::getline(ls, grid, ',');
			vTile[(gy*14)+gx].init(grid, STAGE_X_BEGIN + gx*32, STAGE_Y_BEGIN + gy*32, (gy * 14) + gx);
		}
	}
}

void StageLoader::update()
{
	for (int i = 0; i < pTileSlot; i++)
	{
		vTile[i].update();
	}
}