#include "stage.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

void Stage::getStageFromFile(int stage_no, TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players)
{
	std::ifstream stageFile("stage/stage_" + std::to_string(stage_no) + ".csv");

	tiles.vTile = std::vector<TileEntity>(tiles.pTileSlot, TileEntity());

	if (!stageFile.is_open() && pLastStage == 0)
	{
		pLastStage = stage_no - 1;
		stageFile.open("stage/stage_" + std::to_string(stage_no % pLastStage) + ".csv");
	}

	if (!stageFile.is_open())
		throw std::runtime_error("Could not open file");

	std::string line;

	for (int gy = 0; gy < 18; gy++)
	{
		std::getline(stageFile, line);
		std::stringstream ls(line);
		std::string grid;

		for (int gx = 0; gx < 14; gx++)
		{
			std::getline(ls, grid, ',');
			tiles.vTile[(gy * 14) + gx].init(grid, STAGE_X_BEGIN + gx * 32, STAGE_Y_BEGIN + gy * 32, (gy * 14) + gx);

			if (grid == "S")
				enemies.insertSpawnPoint((gy * 14) + gx);
			if (grid == "1")
				players.p1.setPosition(gridX(gx),gridY(gy));
			if (grid == "2")
				players.p2.setPosition(gridX(gx), gridY(gy));

		}
	}
}

void Stage::setAllEnemiesMovement(TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players)
{
	for (int ei = 0; ei < enemies.pEnemySlot; ei++)
	{
		if (enemies.vEnemy[ei].getActive() == true)
		{
			if((enemies.vEnemy[ei].getPosition().x - 16) % 32 == 0 && (enemies.vEnemy[ei].getPosition().y - 32) % 32 == 0)
				setEnemyMovement(tiles, enemies, players, ei);
		}
	}
}

void Stage::setEnemyMovement(TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players, int ei)
{

	SDL_Rect enemyPos = enemies.vEnemy[ei].getPosition();
	SDL_Rect playerPos;
	bool p1Active = players.p1.getActive();
	bool p2Active = players.p2.getActive();

	if (p1Active == true && p2Active == true && ei % 2 == 0)
	{
		playerPos = players.p1.getPosition();
	}
	else if (p1Active == true && p2Active == true)
	{
		playerPos = players.p2.getPosition();
	}
	else if (p1Active == true)
	{
		playerPos = players.p1.getPosition();
	}
	else if (p2Active == true)
	{
		playerPos = players.p2.getPosition();
	}
	else
	{
		enemies.vEnemy[ei].clearMovement();
		return;
	}

	int egx = (enemyPos.x - STAGE_X_BEGIN) / 32;
	int egy = (enemyPos.y - STAGE_Y_BEGIN) / 32;
	int pgx = (playerPos.x - STAGE_X_BEGIN) / 32;
	int pgy = (playerPos.y - STAGE_Y_BEGIN) / 32;
	//printf("player %d,%d\n",pgx,pgy);

	Cell q;
	q.dir = IDLE;
	q.currentX = egx;
	q.currentY = egy;
	q.parentX = -1;
	q.parentY = -1;
	q.g = 0;
	q.h = getEstimateDistance(egx, egy, pgx, pgy);
	
	vOpenList.push_back(q);
	
	//printf("listing closed list\n");
	do
	{
		int iw = findTheLeastWeightCell();
		q = vOpenList[iw];
		if(vOpenList.size() > iw) vOpenList.erase(vOpenList.begin() + iw);
		addQSuccessorToOpenList(tiles, q, pgx, pgy);
		//printf("dir:%d %d,%d->%d,%d\n", q.dir, q.parentX, q.parentY, q.currentX, q.currentY);
		vCloseList.push_back(q);
	} while (q.h > 0);

	//printf("only linked to parent\n");
	
	if(vCloseList.size() >= 2)
	{
		for (int i = vCloseList.size() - 2; i >= 0; i--)
		{
			if (vCloseList[i].currentX == q.parentX && vCloseList[i].currentY == q.parentY)
			{
				q = vCloseList[i];
			}
			else
			{
				if (vCloseList.size() > i) vCloseList.erase(vCloseList.begin() + i);
			}
		}
	}

	//for(int i = 0; i < vCloseList.size(); i++)
		//printf("dir:%d %d,%d->%d,%d\n", vCloseList[i].dir, vCloseList[i].parentX, vCloseList[i].parentY, vCloseList[i].currentX, vCloseList[i].currentY);
	
	
	//printf("add movement to an enemy\n");
	Facing currentDir = IDLE;
	enemies.vEnemy[ei].clearMovement();
	if (vCloseList.size() >= 2)
	{
		currentDir = vCloseList[1].dir;
		for (int i = 2; i < vCloseList.size() - 1; i++)
		{
			if (vCloseList[i].dir != currentDir)
			{
				int x = STAGE_X_BEGIN + (vCloseList[i].parentX * 32);
				int y = STAGE_Y_BEGIN + (vCloseList[i].parentY * 32);
				enemies.vEnemy[ei].addMovement(currentDir, x, y);
				//printf("dir:%d to:%d,%d\n", currentDir, x, y);
				currentDir = vCloseList[i].dir;
			}
		}

		if ((vCloseList[vCloseList.size() - 2].dir != vCloseList[vCloseList.size() - 1].dir) && (vCloseList[vCloseList.size() - 2].dir != IDLE))
		{
			currentDir = vCloseList[vCloseList.size() - 2].dir;
			int x = STAGE_X_BEGIN + (vCloseList[vCloseList.size() - 1].parentX * 32);
			int y = STAGE_Y_BEGIN + (vCloseList[vCloseList.size() - 1].parentY * 32);
			enemies.vEnemy[ei].addMovement(currentDir, x, y);
			//printf("dir:%d to:%d,%d\n", currentDir, x, y);
		}
	}
	
	if (vCloseList.size() >= 1)
	{
		currentDir = vCloseList[vCloseList.size() - 1].dir;
		int x = STAGE_X_BEGIN + (vCloseList[vCloseList.size() - 1].currentX * 32);
		int y = STAGE_Y_BEGIN + (vCloseList[vCloseList.size() - 1].currentY * 32);
		enemies.vEnemy[ei].addMovement(currentDir, x, y);
		//printf("dir:%d to:%d,%d\n", currentDir, x, y);
	}
 	enemies.vEnemy[ei].updateMoveList();

	vCloseList.clear();
	vOpenList.clear();
}

float Stage::getEstimateDistance(int egx, int egy, int pgx, int pgy)
{
	float x = (float)pgx - egx;
	float y = (float)pgy - egy;

	return sqrt(x * x + y * y);
}

int Stage::findTheLeastWeightCell()
{
	int lowestIndex = -1;
	float lowestF = INT_MAX;
	for (int i = 0; i < vOpenList.size(); i++)
	{
		if (vOpenList[i].g + vOpenList[i].h < lowestF)
		{
			lowestIndex = i;
			lowestF = vOpenList[i].g + vOpenList[i].h;
		}
	}
	return lowestIndex;
}

void Stage::addQSuccessorToOpenList(TileGroup tiles, Cell q, int pgx, int pgy)
{
	Cell qS;
	int diffX[4] = { -1, 1, 0, 0 };
	int diffY[4] = { 0, 0, -1, 1 };
	Facing dir[4] = {FACING_LEFT, FACING_RIGHT, FACING_UP, FACING_DOWN};

	int order[4] = {0, 1, 2, 3};
	std::random_shuffle(std::begin(order), std::end(order));
	

	for (int i = 0; i < 4; i++)
	{
		int oi = order[i];
		if ((q.currentX + diffX[oi]) >= 0 && (q.currentX + diffX[oi]) < 14 && (q.currentY + diffY[oi]) >= 0 && (q.currentY + diffY[oi]) < 18)
		{
			if (tiles.vTile[(q.currentY + diffY[oi]) * 14 + q.currentX + diffX[oi]].getPassable() == true)
			{
				qS.dir = dir[oi];
				qS.currentX = q.currentX + diffX[oi];
				qS.currentY = q.currentY + diffY[oi];
				qS.parentX = q.currentX;
				qS.parentY = q.currentY;
				qS.g = q.g + 1;
				qS.h = getEstimateDistance(qS.currentX,qS.currentY,pgx,pgy);
				
				if (checkCloseList(qS) == false)
				{
					updateSuccessorToOpenList(qS);
				}
			}
		}
	}
}

void Stage::updateSuccessorToOpenList(Cell qS)
{
	for (int i = 0; i < vOpenList.size(); i++)
	{
		if (vOpenList[i].currentX == qS.currentX && vOpenList[i].currentY == qS.currentY)
		{
			if (vOpenList[i].g + vOpenList[i].h <= qS.g + qS.h)
			{
				return; //do nothing
			}
			else if (vOpenList[i].g + vOpenList[i].h > qS.g + qS.h)
			{
				vOpenList[i] = qS; //update better one
				return;
			}
		}
	}

	vOpenList.push_back(qS);
	return;
}

bool Stage::checkCloseList(Cell qS)
{
	int limit;
	if (vCloseList.size() > 2)
		limit = vCloseList.size() - 1;
	else
		limit = vCloseList.size();

	for (int i = 0; i < limit; i++)
	{
		if (vCloseList[i].currentX == qS.currentX && vCloseList[i].currentY == qS.currentY)
		{
			return true;
		}
	}
	return false;
}