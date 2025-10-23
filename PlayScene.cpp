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
	/*Player* pPlayer = new Player(this);
	pPlayer->Initialize();
	childList_.push_back(pPlayer);*/

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
