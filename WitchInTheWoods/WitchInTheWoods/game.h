#pragma once
#include <vector>
#include "load.h"
#include "entity_bullet.h"
#include "entity_player.h"
#include "entity_enemy.h"
#include "entity_tile.h"
#include "stage.h"
#include "profile.h"

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
	virtual void sceneTransition();

private:
	GameScene* mScene;
};

class GameScene : public Game
{
public:
	virtual ~GameScene();
	virtual GameScene* handleEvent(Game& game, SDL_Event* e);
	virtual void update(Game& game);
	virtual GameScene* sceneTransition(Game& game);
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
	GameScene* sceneTransition(Game& game);

private:
	PlayerGroup players;
	TileGroup tiles;
	BulletGroup bullets;
	Stage stage;
	EnemySpawner enemies;

	int time = 0;
};