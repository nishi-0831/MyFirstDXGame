#include "EnemyManager.h"
#include "MoveCommand.h"
#include "Player.h"
#include "Engine/GameTime.h"
#include "Engine/SceneManager.h"
#include <chrono>
namespace
{
	const unsigned int GENERATE_ENEMY_NUM{10};
	unsigned int generatedEnemyCount{ 0 };
	// 現在時刻をシード値に
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	// 謎の乱数生成器
	std::mt19937 gen(seed);
}



EnemyManager::EnemyManager(GameObject* parent)
	: GameObject(parent,"EnemyManager")
	,  generateInterval_{2}
	, elapsed_{0.0f}
{
	
}

GameObject* EnemyManager::GenerateEnemy()
{
	if (generatedEnemyCount > GENERATE_ENEMY_NUM)
	{
		// TODO: 生成し終わった時でなく、破壊され終わった後などにする
		dynamic_cast<SceneManager*>(FindObject("SceneManager"))->ChangeScene(SCENE_ID_RESULT);

		return nullptr;
	}
	Player* player = Instantiate<Player>(this);
	generatedEnemyCount++;

	float minX = 4.0f;
	float maxX = 6.0f;
	float minY = -1.0f;
	float maxY = 1.0f;
	std::uniform_int_distribution<> x(minX, maxX);
	std::uniform_int_distribution<> y(minY, maxY);
	player->transform_.position.x = x(gen);
	player->transform_.position.y = y(gen);

	float distXMin = 0.0f;
	float distXMax = 0.2f;
	std::uniform_int_distribution<> distX(distXMin, distXMax);
	
	MoveCommand* moveCommand = new MoveCommand(distX(gen));

	commandManager_.Set(moveCommand, player);
	return player;
}

void EnemyManager::Initialize()
{
	GenerateEnemy();
}

void EnemyManager::Update()
{
	commandManager_.Update();
	elapsed_ += GameTime::DeltaTime();
	if (elapsed_ >= generateInterval_)
	{
		GenerateEnemy();
		elapsed_ = 0.0f;
	}
}

void EnemyManager::Draw()
{
}

void EnemyManager::Release()
{
}
