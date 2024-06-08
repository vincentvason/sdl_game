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

void Profile::addScore(int score, int player)
{
	pScore[player] += score;
}

void Profile::decrementLife(int player)
{
	pLife[player]--;
}