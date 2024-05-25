#pragma once
#include <vector>
#include "load.h"
#include "entity_bullet.h"
#include "entity_player.h"
#include "entity_enemy.h"


class Game;
class GameScene;
class MenuScene;
class InGameScene;

extern MenuScene menu;
extern InGameScene ingame;

class Game
{
public:
	Game();
	virtual void handleEvent(SDL_Event* e);
	virtual void update();
	void insertCredit();
	bool usedCredit();
	int getHighScore();
	void updateCreditHUD();
	bool getPlayerIn(int player);
	void setPlayerIn(int player, bool val);

private:
	GameScene* mScene;
	int mCredit = 0;
	int mHighScore = 10000;
	bool pIn[2] = { false };
};

class GameScene
{
public:
	virtual ~GameScene();
	virtual GameScene* handleEvent(Game& game, SDL_Event* e);
	virtual void update(Game& game);
};

class MenuScene : public GameScene
{
public:
	GameScene* handleEvent(Game& game, SDL_Event* e);
	void update(Game& game);
};

class InGameScene : public GameScene
{
public:
	GameScene* handleEvent(Game& game, SDL_Event* e);
	void update(Game& game);

private:
	void updatePlayerHUD(Game& game);
	void updateBottomHUD(Game& game);

	int pScore[2] = { 0 }, pLife[2] = { 3 };
	int pStage = 1;

	PlayerEntity p1 = PlayerEntity(&gWitch1Texture);
	EnemyEntity en = EnemyEntity(&gMonster1Texture, 100, 100);
	BulletGroup bullets;
};