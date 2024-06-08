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

	void addScore(int score, int player = 0);

	int getLife(int player) { return pLife[player]; };
	void decrementLife(int player = 0);

	int getCredit() { return mCredit; };
	int getHighScore() { return mHighScore; };
	bool getPlayerIn(int player) { return pIn[player]; };
	void setPlayerIn(int player, bool status) { pIn[player] = status; };


private:
	int pScore[2] = { 0 }, pLife[2] = { 3 };
	int pStage = 1;
	bool isStageLoaded = false;

	int mCredit = 0;
	int mHighScore = 10000;
	bool pIn[2] = { false };
};

extern Profile profile;
