#include "PlayScene.h"
#include "Enemy.h"
PlayScene::PlayScene(GameObject* pParent)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Instantiate<Enemy>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
	
}

void PlayScene::Release()
{
}
