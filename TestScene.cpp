#include "TestScene.h"
#include "ChildOden.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Character.h"
#include "Player.h"
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
	b = Instantiate<Player>(this);
	Instantiate<EnemyManager>(this);
}

void TestScene::Update()
{
	if (Input::IsMouseButtonDown(1))
	{
		/*GameObject* obj = FindObject("SceneManager");
		SceneManager* sceneManager = dynamic_cast<SceneManager*>(obj);
		if (sceneManager == nullptr) return;

		sceneManager->ChangeScene(SCENE_ID_PLAY);*/
		a->RoundRobin(b);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
