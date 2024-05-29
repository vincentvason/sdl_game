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

void StageEntity::init(std::string terrainType, int x, int y)
{
	if (terrainType == "E")
	{
		isActive = true;
		mSprite = &gEdificeTexture;
		isPassable = false;

	}
	else if (terrainType == "W")
	{
		isActive = true;
		mSprite = &gWallTexture;
		isPassable = false;
	}
	else if (terrainType == "T")
	{
		isActive = true;
		mSprite = &gTreeTexture;
		isPassable = true;
	}

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

void StageEntity::update()
{
	if (isActive == true)
	{
		SDL_Rect clip = { 32 * (int)mAnimState, 0, 32, 32 };
		mSprite->render(mCollider.x, mCollider.y, &clip);
	}	
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
			printf("%d,%d: %s\n",gx,gy,grid.c_str());
			vTile[(gy*14)+gx].init(grid, STAGE_X_BEGIN + gx*32, STAGE_Y_BEGIN + gy*32);
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