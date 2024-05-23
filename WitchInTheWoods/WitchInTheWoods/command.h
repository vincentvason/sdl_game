#pragma once
#include "game.h"
#include "entity_player.h"

class Command
{
public:
	virtual ~Command() {};
	virtual void execute(Game& game) = 0;
};

class InsertCoinCommand : public Command
{
public:
	void execute(Game& game);
};

class StartCommand : public Command
{
public:
	void execute(Game& game, int playerIndex);
};