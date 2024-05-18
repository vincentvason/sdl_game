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

void Game::update()
{
	mScene->update(*this);
}

void Game::insertCredit()
{
	if (mCredit < 9) mCredit++;
}

bool Game::usedCredit()
{
	if (mCredit > 0)
	{
		mCredit--;
		return true;
	}
	
	return false;
}

void Game::updateCreditHUD()
{
	char string[11];
	sprintf_s(string, 11, "CREDIT(S)%d", mCredit);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(320, 624);
}

int Game::getHighScore()
{
	return mHighScore;
}

bool Game::getPlayerIn(int player)
{
	return pIn[player];
}

void Game::setPlayerIn(int player, bool val)
{
	pIn[player] = val;
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