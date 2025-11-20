#pragma once
#include "Command.h"

class MoveCommand : public Command
{
public:
	MoveCommand();
	MoveCommand(float distX);
	void Execute(Enemy* _pPlayer) override;
	bool Transition(Enemy* _pPlayer) override;
private:
	float distX_;
	float duration_;
	float elapsed_;
	float speed_;
};