#pragma once
#include <vector>
#include "load.h"
#include "entity_bullet.h"
#include "entity_player.h"


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
	int updateFreeBulletIndex();

	int pScore[2] = { 0 }, pLife[2] = { 3 };
	int pStage = 1;
	int pBulletSlot = 8;
	int pBulletIndex = 0;

	Player p1 = Player(&gWitch1Texture);
	Bullet pBullet = Bullet(&gBulletTexture);
	std::vector<Bullet> vBullet = std::vector<Bullet>(pBulletSlot, Bullet(&gBulletTexture));
};