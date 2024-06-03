#pragma once
#include "entity_enemy.h"
#include "entity_tile.h"
#include "entity_player.h"
#include <stack>

class Stage : public Entity
{
public:
	Stage() {};
	void getStageFromFile(int stage_no, TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players);
	void setAllEnemiesMovement(TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players);

protected:
	struct Cell {
		int currentX = -1, currentY = -1;
		int parentX = -1, parentY = -1;
		Facing dir = FACING_DOWN;
		float g = 0, h = 0; // f = g(distance from start) + h(estimated distance to finish)
	};

	void setEnemyMovement(TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players, int ei);
	float getEstimateDistance(int egx, int egy, int pgx, int pgy);
	int findTheLeastWeightCell();
	void addQSuccessorToOpenList(TileGroup tiles, Cell q, int pgx, int pgy);
	void updateSuccessorToOpenList(Cell qS);
	bool checkCloseList(Cell qS);

	std::vector<Cell> vOpenList;
	std::vector<Cell> vCloseList;
	int pLastStage = 0;
	
};