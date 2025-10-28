#include "TestScene.h"
#include "ChildOden.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Character.h"
TestScene::TestScene(GameObject* pParent)
	: GameObject(pParent,"TestScene")
{
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	Instantiate<Character>(this);
}

void TestScene::Update()
{
	if (Input::IsMouseButtonDown(1))
	{
		GameObject* obj = FindObject("SceneManager");
		SceneManager* sceneManager = dynamic_cast<SceneManager*>(obj);
		if (sceneManager == nullptr) return;

		sceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
