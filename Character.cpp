#include "Character.h"
#include "Engine/SceneManager.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"
#include "Bullet.h"
namespace
{
	DirectX::XMVECTOR BULLET_OFFSET{ 0,0.25,0 };
}
Character::Character(GameObject* parent)
	: GameObject(parent,"Character")
{
}

Character::~Character()
{
}

void Character::Initialize()
{
	hModel_ = Model::Load("character.fbx");

	pCollider_ = new SphereCollider(0.5f);
	pCollider_->SetTransform(&transform_);
	transform_.position.x = 0.0f;
	transform_.scale.x = 0.2f;
	transform_.scale.y = 0.2f;
	transform_.scale.z = 0.2f;
}

void Character::Update()
{
	//transform_.rotate.y += 1.0f;
	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.position.x += 0.02f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.position.x -= 0.02f;
	}
	if (Input::IsKey(DIK_UP))
	{
		transform_.position.y += 0.02f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.position.y -= 0.02f;
	}
	if (Input::IsKeyDown(DIK_SPACE))
	{
		Shot();
	}
}

void Character::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Character::Release()
{
}

void Character::OnCollision(GameObject* pOther)
{
}

void Character::Shot()
{
	SceneManager* sceneManager = dynamic_cast<SceneManager*>( FindObject("SceneManager"));
	Bullet* bullet = Instantiate<Bullet>(sceneManager->GetCurrentScene());
	DirectX::XMVECTOR vBulletPos = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&transform_.position), BULLET_OFFSET);
	DirectX::XMFLOAT3 bulletPos;
	DirectX::XMStoreFloat3(&bulletPos, vBulletPos);
	bullet->SetParam(bulletPos, transform_.Forward());
}
