#include "PlayScene.h"
#include "Player.h"
PlayScene::PlayScene(GameObject* pParent)
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Instantiate<Player>(this);
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
