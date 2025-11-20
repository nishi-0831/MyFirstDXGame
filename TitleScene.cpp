#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* pParent)
	: GameObject(pParent,"TitleScene")
{
	pSprite_ = new Sprite();
	pSprite_->Load("Assets/titleImage.png");
}

TitleScene::~TitleScene()
{
	SAFE_RELEASE(pSprite_);
}

void TitleScene::Initialize()
{
	pSprite_->Initialze();
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* sceneManager = dynamic_cast<SceneManager*>(FindObject("SceneManager"));
		sceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

void TitleScene::Draw()
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	pSprite_->Draw(mat);
}

void TitleScene::Release()
{
}
