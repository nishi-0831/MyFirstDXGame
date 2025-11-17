#pragma once
#include "Engine/GameObject.h"
#include "Command.h"
#include "MoveCommand.h"
#include <random>

// Enemy‚ğ¶¬‚·‚é
// Enemy‚Ì¶¬A”jŠü‚ğŒŸ’m‚µƒJƒEƒ“ƒg‚ğ‚¢‚¶‚é
// 


class EnemyManager : public GameObject
{
public:
	EnemyManager(GameObject* parent);
	GameObject* GenerateEnemy();
	// GameObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	
private:
	//MoveCommand* moveCommand;
	unsigned int enemyCounter_; // “G‚Ì”
	CommandManager commandManager_;
	unsigned int generateInterval_;
	float elapsed_;
};