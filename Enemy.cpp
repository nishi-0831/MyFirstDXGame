#include "Enemy.h"
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
Enemy::Enemy(GameObject* parent)
	: GameObject(parent, "Enemy")
{
}

Enemy::~Enemy()
{
	Notify(Event::Destroyed);

}

void Enemy::Initialize()
{
	hModel_ = Model::Load("oden.fbx");

	pCollider_ = new SphereCollider(0.5f);
	pCollider_->SetOffset(DirectX::XMVectorSet(0, 1.5, 0, 0));
	pCollider_->SetTransform(&transform_);

	transform_.scale.x = 0.35f;
	transform_.scale.y = 0.35f;
	transform_.scale.z = 0.35f;
	transform_.position.x = 1.0f;
	GameObject* child1 = Instantiate<ChildOden>(this);
	child1->SetPosition(-1.0f, 0.0f, 0.0f);

	GameObject* child2 = Instantiate<ChildOden>(this);
	child2->SetPosition(1.0f, 0.0f, 0.0f);
}

void Enemy::Update()
{
	transform_.rotate.y += 1.0f;
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
	if (pFbx_)
	{
		pFbx_->Release();
		delete pFbx_;
		pFbx_ = nullptr;
	}
}

void Enemy::OnCollision(GameObject* pOther)
{
	
}

void Enemy::AddObserver(IObserver* pObserver)
{
	observers_.push_back(pObserver);
}

void Enemy::Notify(Event event)
{
	for (IObserver* observer : observers_)
	{
		observer->OnNotify(this, event);
	}
}
