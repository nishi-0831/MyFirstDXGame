#include "Command.h"
#include "Engine/ReleaseUtility.h"
void CommandManager::Update()
{
	for (CommandEnemyPair& commandEnemyPair : commandEnemyPairs_)
	{
		Command*& command = commandEnemyPair.first;
		Player* player = commandEnemyPair.second;

		if (command == nullptr)
			continue;

		command->Execute(player);
		if (command->Transition(player))
		{
			// TODO: command‚Ì‰ð•ú
			command = command->nextCommand_;
		}
	}
}

void CommandManager::Set(Command* _pCommand, Player* _pEnemy)
{
	commandEnemyPairs_.push_back(std::make_pair(_pCommand, _pEnemy));
}

Command::~Command()
{
	SAFE_DELETE(nextCommand_);
}
