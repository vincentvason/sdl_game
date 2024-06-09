#include "load.h"
#include "game.h"

//Static State for each scene
MenuScene menu;
InGameScene ingame;

Game::Game()
{
	this->mScene = &menu;
}

void Game::handleEvent(SDL_Event* e)
{
	GameScene* scene = mScene->handleEvent(*this, e);
	if (scene != nullptr)
	{
		mScene = scene;
	}
}

void Game::sceneTransition()
{
	GameScene* scene = mScene->sceneTransition(*this);
	if (scene != nullptr)
	{
		mScene = scene;
	}
}

void Game::update()
{
	mScene->update(*this);
}

GameScene::~GameScene()
{

}

GameScene* GameScene::handleEvent(Game& game, SDL_Event* e)
{
	return nullptr;
}

void GameScene::update(Game& game)
{

}

GameScene* GameScene::sceneTransition(Game& game)
{
	return nullptr;
}