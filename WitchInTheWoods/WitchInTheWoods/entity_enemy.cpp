#include "entity_enemy.h"
#include "profile.h"
#include <cmath>
#include <cstdlib>

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
	checkMoveList();

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

	checkBorderCollision();
	checkStageCollision(tiles.vTile);
	if (checkEnemiesCollision(enemies) == true)
	{
		mCollider.x -= mVelX;
		mCollider.y -= mVelY;
	}
	

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

void EnemyEntity::checkBorderCollision()
{
	if (mCollider.x + mVelX < STAGE_X_BEGIN) mCollider.x = STAGE_X_BEGIN;
	else if (mCollider.x + mVelX > STAGE_X_END) mCollider.x = STAGE_X_END;
	else mCollider.x += mVelX;

	if (mCollider.y + mVelY < STAGE_Y_BEGIN) mCollider.y = STAGE_Y_BEGIN;
	else if (mCollider.y + mVelY > STAGE_Y_END) mCollider.y = STAGE_Y_END;
	else mCollider.y += mVelY;
}

void EnemyEntity::checkStageCollision(std::vector<TileEntity> vTile)
{
	float gx = (mCollider.x - STAGE_X_BEGIN) / 32.0;
	float gy = (mCollider.y - STAGE_Y_BEGIN) / 32.0;


	int gCheck[4] = {
		((std::floor(gy) * 14) + std::floor(gx)),
		((std::floor(gy) * 14) + std::ceil(gx)),
		((std::ceil(gy) * 14) + std::floor(gx)),
		((std::ceil(gy) * 14) + std::ceil(gx))
	};

	for (int i = 0; i < 4; i++)
	{
		if (gCheck[i] >= 0 && gCheck[i] < 14 * 18)
		{
			SDL_Rect rectCheck = vTile[gCheck[i]].getPosition();

			//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0x00);
			//SDL_RenderFillRect(gRenderer, &rectCheck);

			if (vTile[gCheck[i]].getPassable() == false && Entity::checkCollision(rectCheck, mCollider) == true)
			{
				mCollider.x -= mVelX;
				mCollider.y -= mVelY;
			}
		}
	}
}

bool EnemyEntity::checkEnemiesCollision(EnemySpawner enemies)
{
	SDL_Rect dest;
	dest.x = mCollider.x;
	dest.y = mCollider.y;
	dest.w = 32;
	dest.h = 32;

	for (int i = 0; i < enemies.pEnemySlot; i++)
	{
		SDL_Rect comp = enemies.vEnemy[i].getPosition();
		if (std::abs(comp.x-dest.x) < 2 && std::abs(comp.y-dest.y) < 2)
		{
			//skipped
		}
		else
		{
			//printf("i=%d / comp=%d,%d / self=%d,%d\n", i, comp.x, comp.y, dest.x, dest.y);
			if ((enemies.vEnemy[i].getPosition().x - 16) % 32 == 0 && (enemies.vEnemy[i].getPosition().y - 32) % 32 == 0)
			{
				if (checkCollision(dest, comp))
				{
					return true;
				}
			}
			else
			{
				if (checkCollision(dest, comp))
				{
					
				}
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
		if (profile.getLife(0) > 0 || profile.getLife(1) > 0)
		{
			vEnemy[i].move(enemies, tiles);
		}
		vEnemy[i].update(tiles);
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