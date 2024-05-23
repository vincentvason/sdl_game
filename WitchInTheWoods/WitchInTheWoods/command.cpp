#include "command.h"

void InsertCoinCommand::execute(Game& game)
{
	game.insertCredit();
}

void StartCommand::execute(Game& game, int playerIndex)
{
	if (game.getPlayerIn(playerIndex) == false)
	{ 
		if (game.usedCredit())
		{
			game.setPlayerIn(playerIndex, true);
		}
	}
}