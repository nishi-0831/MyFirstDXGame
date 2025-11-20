#include "EnemyManager.h"
#include "MoveCommand.h"
#include "Enemy.h"
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
	, clearDestroyCount_{10}
{
	
}

GameObject* EnemyManager::GenerateEnemy()
{
	Enemy* player = Instantiate<Enemy>(this);
	player->AddObserver(this);
	generatedEnemyCount++;

	int minX = 300;
	int maxX = 400;
	int minY = -100;
	int maxY = 100;
	std::uniform_int_distribution<> x(minX, maxX);
	std::uniform_int_distribution<> y(minY, maxY);
	player->transform_.position.x = x(gen) / 100.0f;
	player->transform_.position.y = y(gen) / 100.0f;

	int distXMin = 0;
	int distXMax = 100;
	std::uniform_int_distribution<> distX(distXMin, distXMax);
	float dist = distX(gen) / 100.0f;
	MoveCommand* moveCommand = new MoveCommand(dist);

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

void EnemyManager::OnNotify(GameObject* pGameObj, Event event)
{
	if (event == Event::Destroyed)
	{
		destroyedCount_++;
	}
	if (destroyedCount_ >= clearDestroyCount_)
	{
		dynamic_cast<SceneManager*>(FindObject("SceneManager"))->ChangeScene(SCENE_ID_RESULT);
	}
}

