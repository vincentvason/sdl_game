#include "stage.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

void Stage::getStageFromFile(int stage_no, TileGroup& tiles, EnemySpawner& enemies, PlayerGroup& players)
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

	Cell q;
	q.dir = IDLE;
	q.currentX = egx;
	q.currentY = egy;
	q.parentX = -1;
	q.parentY = -1;
	q.g = 0;
	q.h = getEstimateDistance(egx, egy, pgx, pgy);
	
	vOpenList.push_back(q);

//	printf("listing closed list\n");
	do
	{
		int iw = findTheLeastWeightCell();
		q = vOpenList[iw];
		vOpenList.erase(vOpenList.begin() + iw);
		addQSuccessorToOpenList(tiles, q, pgx, pgy);
		vCloseList.push_back(q);
	} while (q.h > 0);

//	printf("only linked to parent\n");
	for (int i = vCloseList.size() - 2; i >= 0; i--)
	{
		if (vCloseList[i].currentX == q.parentX && vCloseList[i].currentY == q.parentY)
		{
			q = vCloseList[i];
		}
		else
		{
			vCloseList.erase(vCloseList.begin() + i);
		}
	}

	Facing currentDir = IDLE;
	if (vCloseList.size() >= 2)
	{
		currentDir = vCloseList[1].dir;
		enemies.vEnemy[ei].clearMovement();
		for (int i = 2; i < vCloseList.size() - 1; i++)
		{
			if (vCloseList[i].dir != currentDir)
			{
				int x = STAGE_X_BEGIN + (vCloseList[i].parentX * 32);
				int y = STAGE_Y_BEGIN + (vCloseList[i].parentY * 32);
				enemies.vEnemy[ei].addMovement(currentDir, x, y);
				//printf("dir:%d to:%d,%d\n", currentDir, vCloseList[i].parentX, vCloseList[i].parentY);
				currentDir = vCloseList[i].dir;
			}
		}

		if ((vCloseList[vCloseList.size() - 2].dir != vCloseList[vCloseList.size() - 1].dir))
		{
			currentDir = vCloseList[vCloseList.size() - 2].dir;
			int x = STAGE_X_BEGIN + (vCloseList[vCloseList.size() - 1].parentX * 32);
			int y = STAGE_Y_BEGIN + (vCloseList[vCloseList.size() - 1].parentY * 32);
			enemies.vEnemy[ei].addMovement(currentDir, x, y);
			//printf("dir:%d to:%d,%d\n", currentDir, vCloseList[vCloseList.size() - 1].currentX, vCloseList[vCloseList.size() - 1].currentY);
		}
	}
	if (vCloseList.size() >= 1)
	{
		currentDir = vCloseList[vCloseList.size() - 1].dir;
		int x = STAGE_X_BEGIN + (vCloseList[vCloseList.size() - 1].currentX * 32);
		int y = STAGE_Y_BEGIN + (vCloseList[vCloseList.size() - 1].currentY * 32);
		enemies.vEnemy[ei].addMovement(currentDir, x, y);
		//printf("dir:%d to:%d,%d\n", currentDir, vCloseList[vCloseList.size() - 1].currentX, vCloseList[vCloseList.size() - 1].currentY);
	}

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

	for (int i = 0; i < 4; i++)
	{
		if ((q.currentX + diffX[i]) >= 0 && (q.currentX + diffX[i]) < 14 && (q.currentY + diffY[i]) >= 0 && (q.currentY + diffY[i]) < 18)
		{
			if (tiles.vTile[(q.currentY + diffY[i]) * 14 + q.currentX + diffX[i]].getPassable() == true)
			{
				qS.dir = dir[i];
				qS.currentX = q.currentX + diffX[i];
				qS.currentY = q.currentY + diffY[i];
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