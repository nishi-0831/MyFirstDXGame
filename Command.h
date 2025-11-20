#pragma once
#include <tuple>
#include <vector>
#include "Enemy.h"

class Command
{
public:
	virtual ~Command();
	virtual void Execute(Enemy* _pPlayer) = 0;
	virtual bool Transition(Enemy* _pPlayer) = 0;
	Command* nextCommand_;
}; 

class CommandManager
{
public:
	void Update();
	void Set(Command* _pCommand, Enemy* _pEnemy);
private:
	using CommandEnemyPair = std::pair<Command*, Enemy*>;
	std::vector<CommandEnemyPair> commandEnemyPairs_;
};