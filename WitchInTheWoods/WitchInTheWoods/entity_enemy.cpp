#include "entity_enemy.h"
#include "profile.h"
#include <cmath>
#include <cstdlib>
#include <random>
#include <algorithm>

EnemyEntity::EnemyEntity(LTexture* sprite, int x, int y)
{
	isActive = true;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;
}

SDL_Rect EnemyEntity::getPosition()
{
	return mCollider;
}

Entity::Facing EnemyEntity::getFacing()
{
	return mAnimFacing;
}

bool EnemyEntity::getActive()
{
	return isActive;
}

void EnemyEntity::setActive(bool active)
{
	isActive = active;
}

void EnemyEntity::init(LTexture* sprite, int x, int y)
{
	isActive = true;
	mSprite = sprite;

	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = 32;
	mCollider.h = 32;

	mWaitTime = 200;
	mMoveFrame = 0;
}

void EnemyEntity::update(TileGroup tiles)
{

	int anim;
	SDL_RendererFlip flip;
	
	switch (mAnimFacing)
	{
	case FACING_DOWN:
		anim = 0;
		flip = SDL_FLIP_NONE;
		break;
	case FACING_UP:
		anim = 0;
		flip = SDL_FLIP_VERTICAL;
		break;
	case FACING_LEFT:
		anim = 4;
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case FACING_RIGHT:
		anim = 4;
		flip = SDL_FLIP_NONE;
		break;
	}

	SDL_Rect clip = { 32 * (anim + (int)mAnimWalking), 0, 32, 32 };
	
	if (isActive == true)
	{
		//printf("%d, %d, %s\n", mCollider.x, mCollider.y, mSprite->getFilePath().c_str());
		mSprite->render(mCollider.x, mCollider.y, &clip, 0, 0, flip);
	}
}


void EnemyEntity::move(EnemySpawner enemies, TileGroup tiles)
{
	mMoveFrame++;

	if (mMoveFrame % 3 == 0)
	{
		return;
	}
	

	if (mStuckFrame > STUCK_LIMIT)
	{
		addRandomMovement(tiles, 1);
		mStuckFrame = 0;
	}
	else
	{
		checkMoveList();
	}
	

	switch(mMove)
	{
	case IDLE:
		mAnimFacing = FACING_DOWN;
		mVelX = 0;
		mVelY = 0;
		break;
	case FACING_DOWN:
		mAnimFacing = FACING_DOWN;
		mVelX = 0;
		if (mCollider.y + DEFAULT_VEL > mDestY)
		{
			mVelY = mDestY - mCollider.y;
			nextMovement();
		}
		else
		{
			mVelY = DEFAULT_VEL;
		}
		break;
	case FACING_UP:
		mAnimFacing = FACING_UP;
		mVelX = 0;
		if (mCollider.y - DEFAULT_VEL < mDestY)
		{
			mVelY = mCollider.y - mDestY;
			nextMovement();
		}
		else
		{
			mVelY = -DEFAULT_VEL;

		}
		break;
	case FACING_RIGHT:
		mAnimFacing = FACING_RIGHT;
		mVelY = 0;
		if (mCollider.x + DEFAULT_VEL > mDestX)
		{
			mVelX = mDestX - mCollider.x;
			nextMovement();
		}
		else
		{
			mVelX = DEFAULT_VEL;
		}
		break;
	case FACING_LEFT:
		mAnimFacing = FACING_LEFT;
		mVelY = 0;
		if (mCollider.x - DEFAULT_VEL < mDestX)
		{
			mVelX = mCollider.x - mDestX;
			nextMovement();
		}
		else
		{
			mVelX = -DEFAULT_VEL;
		}
		break;	
	}

	if (snapBorderCollision() == false)
	{
		if (snapStageCollision(tiles.vTile) == false)
		{
			if (snapEnemiesCollision(enemies) == false)
			{
				mCollider.x += mVelX;
				mCollider.y += mVelY;
			}	
		}
	}

	mPastMove = mMove;
	mPastCollider = mCollider;

	if (mVelX != 0 || mVelY != 0)
	{
		mAnimWalking += DEFAULT_STEP_SPEED;
		if (mAnimWalking >= 3) mAnimWalking -= 3;
	}
	else
	{
		mAnimWalking = 0;
	}
}

void EnemyEntity::shoot()
{
	mAnimWalking = 3;	
}

bool EnemyEntity::snapBorderCollision()
{

	if (mCollider.x + mVelX < STAGE_X_BEGIN)
	{
		mCollider.x = STAGE_X_BEGIN;
		return true;
	}
	else if (mCollider.x + mVelX > STAGE_X_END)
	{
		mCollider.x = STAGE_X_END;
		return true;
	}
	else
	{
		mCollider.x += mVelX;
	}

	if (mCollider.y + mVelY < STAGE_Y_BEGIN)
	{
		mCollider.y = STAGE_Y_BEGIN;
		return true;
	}
	else if (mCollider.y + mVelY > STAGE_Y_END)
	{
		mCollider.y = STAGE_Y_END;
		return true;
	}
	else
	{
		mCollider.y += mVelY;
	}

	return false;
}

bool EnemyEntity::snapStageCollision(std::vector<TileEntity> vTile)
{
	float gx = (mCollider.x - STAGE_X_BEGIN) / 32.0;
	float gy = (mCollider.y - STAGE_Y_BEGIN) / 32.0;

	const int SLOT = 12;

	int gCheck[SLOT] = {
		((std::floor(gy - 1) * 14) + std::floor(gx)),
		((std::floor(gy) * 14) + std::floor(gx - 1)),
		((std::floor(gy) * 14) + std::floor(gx)),
		((std::floor(gy - 1) * 14) + std::ceil(gx)),
		((std::floor(gy) * 14) + std::ceil(gx + 1)),
		((std::floor(gy) * 14) + std::ceil(gx)),
		((std::ceil(gy + 1) * 14) + std::floor(gx)),
		((std::ceil(gy) * 14) + std::floor(gx - 1)),
		((std::ceil(gy) * 14) + std::floor(gx)),
		((std::ceil(gy + 1) * 14) + std::ceil(gx)),
		((std::ceil(gy) * 14) + std::ceil(gx + 1)),
		((std::ceil(gy) * 14) + std::ceil(gx))
	};

	SDL_Rect futureEnemyRect;
	futureEnemyRect.x = mCollider.x + mVelX;
	futureEnemyRect.y = mCollider.y + mVelY;
	futureEnemyRect.w = mCollider.w;
	futureEnemyRect.h = mCollider.h;

	for (int i = 0; i < SLOT; i++)
	{
		if (gCheck[i] >= 0 && gCheck[i] < 14 * 18)
		{
			SDL_Rect rectCheck = vTile[gCheck[i]].getPosition();

			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
			//SDL_RenderFillRect(gRenderer, &rectCheck);
			if (vTile[gCheck[i]].getPassable() == false && Entity::checkCollision(rectCheck, futureEnemyRect) == true)
			{
				if (mVelX < 0) mCollider.x = rectCheck.x + 32;
				if (mVelX > 0) mCollider.x = rectCheck.x - 32;
				if (mVelY < 0) mCollider.y = rectCheck.y + 32;
				if (mVelY > 0) mCollider.y = rectCheck.y - 32;
				return true;
			}
		}
	}
	return false;
}

bool EnemyEntity::snapEnemiesCollision(EnemySpawner enemies)
{
	SDL_Rect futureEnemyRect;
	futureEnemyRect.x = mCollider.x + mVelX;
	futureEnemyRect.y = mCollider.y + mVelY;
	futureEnemyRect.w = mCollider.w;
	futureEnemyRect.h = mCollider.h;


	for (int i = 0; i < enemies.pEnemySlot; i++)
	{
		SDL_Rect rectCheck = enemies.vEnemy[i].getPosition();
		if (enemies.vEnemy[i].getActive() == false)
		{
			//skipped
		}
		else if (enemies.vEnemy[i].getWaitTime() > 0)
		{
			//skipped
		}
		else if (std::abs(rectCheck.x-mCollider.x) < 2 && std::abs(rectCheck.y-mCollider.y) < 2)
		{
			//skipped
		}
		else
		{
				if (checkCollision(futureEnemyRect, rectCheck))
				{
					if (mVelX < 0) mCollider.x = rectCheck.x + 32;
					if (mVelX > 0) mCollider.x = rectCheck.x - 32;
					if (mVelY < 0) mCollider.y = rectCheck.y + 32;
					if (mVelY > 0) mCollider.y = rectCheck.y - 32;
					mStuckFrame++;
					return true;
				}
		}
	}

	mStuckFrame = 0;
	return false;
}

bool EnemyEntity::checkEnemiesCollision(EnemySpawner enemies)
{
	for (int i = 0; i < enemies.pEnemySlot; i++)
	{
		SDL_Rect rectCheck = enemies.vEnemy[i].getPosition();
		if (enemies.vEnemy[i].getActive() == false)
		{
			//skipped
		}
		else if (std::abs(rectCheck.x - mCollider.x) < 2 && std::abs(rectCheck.y - mCollider.y) < 2)
		{
			//skipped
		}
		else
		{
			if (checkCollision(mCollider, rectCheck))
			{
				return true;
			}
		}
	}

	return false;
}

void EnemyEntity::checkMoveList()
{
	if (vCommand.size() > 0)
	{
			mMove = vCommand[0].dir;
			mDestX = vCommand[0].posX;
			mDestY = vCommand[0].posY;
	}
	else
	{
		mMove = IDLE;
	}
}

void EnemyEntity::nextMovement()
{
	if (vCommand.size() > 0)
	{
		vCommand.erase(vCommand.begin());
	}
}

void EnemyEntity::addMovement(Entity::Facing dir, int x, int y)
{
	EnemyCommand command;
	command.dir = dir;
	command.posX = x;
	command.posY = y;
	vCommand.push_back(command);
}

void EnemyEntity::addRandomMovement(TileGroup tiles, int range)
{
	int gridX = (mCollider.x - STAGE_X_BEGIN) / 32;
	int gridY = (mCollider.y - STAGE_Y_BEGIN) / 32;
	int fgridX = gridX, fgridY = gridY;
	Facing fdir;
	bool passed = false;

	while (passed == false)
	{
		int dir = std::rand() % 4;
		int dist = std::rand() % range;

		switch (dir)
		{
		case 0:
			fdir = FACING_DOWN;
			fgridY = gridY + dist;
			if (fgridY > 17) fgridY = 17;
		case 1:
			fdir = FACING_LEFT;
			fgridX = gridX - dist;
			if (fgridX < 0) fgridX = 0;
		case 2:
			fdir = FACING_UP;
			fgridY = gridY - dist;
			if (fgridY < 0) fgridY = 0;
		case 3:
			fdir = FACING_RIGHT;
			fgridX = gridX + dist;
			if (fgridX > 13) fgridX = 13;
		}

		passed = tiles.vTile[(fgridY * 14) + fgridX].getPassable();
	}

	addMovement(fdir, fgridX, fgridY);
}

void EnemyEntity::clearMovement()
{
	vCommand.clear();
}

void EnemyEntity::updateMoveList()
{
	//do nothing
}

void EnemySpawner::update(TileGroup tiles, EnemySpawner enemies)
{
	for (int i = 0; i < pEnemySlot; i++)
	{
		if(vEnemy[i].getWaitTime() == 0)
		{
			if (profile.getLife(0) > 0 || profile.getLife(1) > 0)
			{
				vEnemy[i].move(enemies, tiles);
			}
		}

		if ((vEnemy[i].getWaitTime() / 10) % 2 == 0)
		{
			vEnemy[i].update(tiles);
		}
			
		if (vEnemy[i].getWaitTime() > 0)
		{
			if (vEnemy[i].checkEnemiesCollision(enemies) == false)
			{
				vEnemy[i].decrementWaitTime();
			}
			else if(vEnemy[i].getWaitTime() < 8)
			{
				vEnemy[i].setWaitTime(8);
			}
		}
			
	}
}

void EnemySpawner::insertSpawnPoint(int index)
{
	vSpawnPosition.push_back(index);
}

void EnemySpawner::spawnEnemyInOrder(int index)
{
	vEnemy[index].init(&gMonster1Texture,gridX(vSpawnPosition[index]%14),gridY(vSpawnPosition[index]/14));
}

void EnemySpawner::spawnRandomEnemies(int count, int waitTime)
{
	int ncount = count;
	std::vector<int> index;

	if(mWaitTime <= 0 || waitTime == 0)
	{
		for (int i = 0; i < pEnemySlot; i++)
		{
			index.push_back(i);
		}

		std::random_shuffle(std::begin(index), std::end(index));

		for (int i = 0; i < ncount; i++)
		{
			if (vEnemy[index[i]].getActive() == true && ncount < index.size())
			{
				ncount++;
			}
			else if(vEnemy[index[i]].getActive() == false)
			{
				vEnemy[index[i]].init(&gMonster1Texture, gridX(vSpawnPosition[index[i]] % 14), gridY(vSpawnPosition[index[i]] / 14));
			}
		}
		
		mWaitTime = waitTime;
	}
	else
	{
		mWaitTime--;
	}
}

int EnemySpawner::getNumberOfEnemies()
{
	int count = 0;
	for (int i = 0; i < pEnemySlot; i++)
	{
		if (vEnemy[i].getActive() == true)
			count++;
	}

	return count;
}

void EnemySpawner::resetEnemies()
{
	for (int i = 0; i < pEnemySlot; i++)
	{
		vEnemy[i].setActive(false);
	}
}