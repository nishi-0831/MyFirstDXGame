#include "TestScene.h"
#include "ChildOden.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"
namespace
{
	GameObject* a;
	GameObject* b;
}
TestScene::TestScene(GameObject* pParent)
	: GameObject(pParent,"TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	a = Instantiate<Character>(this);
	b = Instantiate<Enemy>(this);
	Instantiate<EnemyManager>(this);
}

void TestScene::Update()
{
	if (Input::IsMouseButtonDown(1))
	{
		a->RoundRobin(b);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
