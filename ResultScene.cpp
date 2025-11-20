#include "ResultScene.h"
#include "Engine/ReleaseUtility.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
ResultScene::ResultScene(GameObject* pParent)
{
	pSprite_ = new Sprite();
	pSprite_->Load("Assets/clearImage.png");
}

ResultScene::~ResultScene()
{
	SAFE_RELEASE(pSprite_);
}

void ResultScene::Initialize()
{
	pSprite_->Initialze();
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	pSprite_->Draw(mat);
}

void ResultScene::Release()
{
}
