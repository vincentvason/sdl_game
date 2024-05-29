#pragma once
#include "entity.h"
#include <vector>

class StageEntity : public Entity
{
public:
	StageEntity();
	StageEntity(LTexture* sprite, int x = 0, int y = 0, bool passable = false);
	void init(std::string terrainType, int x = 0, int y = 0);
	void update();

protected:
	bool isActive = false;
	float mAnimState = 0;
	bool isPassable = false;

	SDL_Rect mCollider = {0, 0, 0, 0};
	LTexture* mSprite = nullptr;
};

class StageLoader : public Entity
{
public:
	StageLoader();
	void getStageFromFile(int stage_no);
	void update();

	int pTileSlot = 14*18;
	std::vector<StageEntity> vTile = std::vector<StageEntity>(pTileSlot, StageEntity());

protected:
	int pLastStage = 0;
};