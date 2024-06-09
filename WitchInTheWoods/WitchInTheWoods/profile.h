#pragma once
#include "load.h"

class Profile {
public:
	enum PlayerIndex {
		PLAYER_1,
		PLAYER_2
	};

	Profile() {};
	void insertCredit();
	bool usedCredit();

	void updateCreditHUD();
	void updatePlayerHUD();
	void updateBottomHUD();
	void updateGameOverHUD();
	void updatePerfectHUD();

	void init();
	void addScore(int score, int player = 0);

	int getLife(int player) { return pLife[player]; };
	void decrementLife(int player = 0);

	int getCredit() { return mCredit; };
	int getHighScore() { return mHighScore; };
	bool getPlayerIn(int player) { return pIn[player]; };
	void setPlayerIn(int player, bool status);

	bool getGameOverStatus() { return isGameOver; };
	void setGameOverStatus(bool st) { isGameOver = st; };

	bool getStageLoadedStatus() { return isStageLoaded; };
	void setStageLoadedStatus(bool st) { isStageLoaded = st; };
	
	int getStage() { return pStage; };
	void incrementStage() { if(pStage < 99999) pStage++; }

	bool getGameOverState() { return mGameOverFreeze < DEFAULT_GAMEOVER_FREEZE_FRAME; };

	void showBonusNotification() { mPerfectFreeze = DEFAULT_PERFECT_FREEZE_FRAME; };

private:
	int DEFAULT_LIFE = 5; //Max 5

	int pScore[2] = { -1 }, pLife[2] = { DEFAULT_LIFE };
	int pStage = 1;

	bool isStageLoaded = false;

	bool isGameOver = false;

	int mCredit = 0;
	int mHighScore = 100;
	bool pIn[2] = { false };

	int DEFAULT_GAMEOVER_COUNTDOWN = 2;
	int DEFAULT_GAMEOVER_FREEZE_FRAME = 300;
	int DEFAULT_PERFECT_FREEZE_FRAME = 200;

	int mGameOverCountdown = DEFAULT_GAMEOVER_COUNTDOWN - 1;
	int mGameOverFreeze = DEFAULT_GAMEOVER_FREEZE_FRAME;
	int mPerfectFreeze = 0;
};

extern Profile profile;
