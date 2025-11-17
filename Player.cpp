#include "Player.h"
#include "ChildOden.h"
#include "Engine/Input.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/SceneManager.h"
#include "Engine/Sprite.h"
namespace
{
}
Player::Player(GameObject* parent)
	: GameObject(parent, "Player")
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("oden.fbx");

	pCollider_ = new SphereCollider(0.5f);
	pCollider_->SetTransform(&transform_);

	transform_.scale.x = 0.7f;
	transform_.scale.y = 0.7f;
	transform_.scale.z = 0.7f;
	transform_.position.x = 1.0f;
	GameObject* child1 = Instantiate<ChildOden>(this);
	child1->SetPosition(-1.0f, 0.0f, 0.0f);

	GameObject* child2 = Instantiate<ChildOden>(this);
	child2->SetPosition(1.0f, 0.0f, 0.0f);
}

void Player::Update()
{
	transform_.rotate.y += 1.0f;

	if (Input::IsMouseButtonDown(Input::MIDDLE))
	{
		KillMe();
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	
}

void Player::Release()
{
	if (pFbx_)
	{
		pFbx_->Release();
		delete pFbx_;
		pFbx_ = nullptr;
	}
}

void Player::OnCollision(GameObject* pOther)
{
	if (pOther->GetName() == "Bullet")
	{
		KillMe();
	}
}
