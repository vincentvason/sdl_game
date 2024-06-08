#include "profile.h"

Profile profile;

void Profile::insertCredit()
{
	if (mCredit < 9) mCredit++;
}

bool Profile::usedCredit()
{
	if (mCredit > 0)
	{
		mCredit--;
		return true;
	}

	return false;
}

void Profile::setPlayerIn(int player, bool status)
{
	if (status == true)
	{
		pLife[player] = 3;
		pScore[player]++;
		mGameOverCountdown = DEFAULT_GAMEOVER_COUNTDOWN - 1;
	}

	pIn[player] = status;
}

void Profile::updateCreditHUD()
{
	char string[11];
	sprintf_s(string, 11, "CREDIT(S)%d", mCredit);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(320, 624);
}

void Profile::updatePlayerHUD()
{
	char string[11];
	SDL_Rect rect;

	if (pIn[0] == true)
	{
		sprintf_s(string, 11, "P1 %07d", pScore[0]);
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 0);

		for (int i = 0; i < 5; i++)
		{
			if (pLife[0] > i)
				rect = { 0, 0, 16, 16 };
			else
				rect = { 16, 0, 16, 16 };
			gIconTexture.render(16 * i, 16, &rect);
		}
	}
	else
	{
		sprintf_s(string, 11, " PRESS TO ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 0);
		sprintf_s(string, 11, "   JOIN   ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(0, 16);
	}

	/*
	if (pIn[1] == true)
	{
		sprintf_s(string, 11, "P2 %07d", pScore[0]);
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 0);

		for (int i = 0; i < 5; i++)
		{
			if (pLife[0] > i)
				rect = { 0, 0, 16, 16 };
			else
				rect = { 16, 0, 16, 16 };
			gIconTexture.render(320 + (16 * i), 16, &rect);
		}
	}
	else
	{
		sprintf_s(string, 11, " PRESS TO ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 0);
		sprintf_s(string, 11, "   JOIN   ");
		gTextTexture.loadFromRenderedText(string, gTextColor_White);
		gTextTexture.render(320, 16);
	}
	*/
}

void Profile::updateBottomHUD()
{
	char string[11];

	sprintf_s(string, 11, "STAGE %03d", pStage);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(0, 608);
	sprintf_s(string, 11, "HI %07d", mHighScore);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	gTextTexture.render(0, 624);
}

void Profile::updateGameOverHUD()
{
	char string[11];
	int count = mGameOverCountdown / (DEFAULT_GAMEOVER_COUNTDOWN / 10);
	int X = 160, Y = 256;

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	sprintf_s(string, 11, "CONTINUE %d", count);
	gTextTexture.loadFromRenderedText(string, gTextColor_White);
	
	SDL_Rect rect = { X, Y, gTextTexture.getWidth(), gTextTexture.getHeight()};

	SDL_RenderDrawRect(gRenderer, &rect);
	SDL_RenderFillRect(gRenderer, &rect);

	gTextTexture.render(X, Y);

	if(mGameOverCountdown > 0) mGameOverCountdown--;
}

void Profile::addScore(int score, int player)
{
	pScore[player] += score;
}

void Profile::decrementLife(int player)
{
	pLife[player]--;
}