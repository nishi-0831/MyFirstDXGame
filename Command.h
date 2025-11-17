#pragma once
#include <tuple>
#include <vector>
#include "Player.h"

class Command
{
public:
	virtual ~Command();
	virtual void Execute(Player* _pPlayer) = 0;
	virtual bool Transition(Player* _pPlayer) = 0;
	Command* nextCommand_;
}; 

class CommandManager
{
public:
	void Update();
	void Set(Command* _pCommand, Player* _pEnemy);
private:
	using CommandEnemyPair = std::pair<Command*, Player*>;
	std::vector<CommandEnemyPair> commandEnemyPairs_;
};