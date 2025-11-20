#include "SceneManager.h"
#include "../PlayScene.h"
#include "../TestScene.h"
#include "../ResultScene.h"
#include "../TitleScene.h"
#include "Direct3D.h"
SceneManager::SceneManager(GameObject* pParent)
	: GameObject(pParent,"SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	currentScene_ = Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_)
	{
		auto scene = childList_.front();
		scene->ReleaseSub();
		SAFE_DELETE(scene);
		childList_.clear();

		switch (nextSceneID_)
		{
		case SCENE_ID_TEST:
			currentScene_ = Instantiate<TestScene>(this);
			break;
		case SCENE_ID_RESULT:
			currentScene_ = Instantiate<ResultScene>(this);
			break;
		case SCENE_ID_TITLE:
			currentScene_ = Instantiate<TitleScene>(this);
			break;
		default:
			break;
		}

		currentSceneID_ = nextSceneID_;

		
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID nextScene)
{
	nextSceneID_ = nextScene;
}

GameObject* SceneManager::GetCurrentScene()
{
	return currentScene_;
}
