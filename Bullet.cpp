#include "Bullet.h"
#include "Engine/Model.h"
#include "Engine/GameTime.h"
Bullet::Bullet(GameObject* pParent)
	: GameObject(pParent,"Bullet")
	, speed_{ 0.03f }
	, lifeSpan_{3.0f}
	, currTime_{0.0f}
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	hModel_ = Model::Load("oden.fbx");
	transform_.scale.x = 0.2f;
	transform_.scale.y = 0.2f;
	transform_.scale.z = 0.2f;

	pCollider_ = new SphereCollider(0.1f);
	pCollider_->SetTransform(&transform_);

}

void Bullet::Update()
{
	//DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir_);
    // XMVECTOR ‚Æ float ‚ÌŠ|‚¯ŽZ‚Í XMVectorScale ‚ðŽg‚¢‚Ü‚·
    DirectX::XMVECTOR movement = DirectX::XMVectorScale(dir_, speed_);
	DirectX::XMVECTOR posVec = DirectX::XMLoadFloat3(&transform_.position);
	DirectX::XMStoreFloat3(&transform_.position, DirectX::XMVectorAdd(posVec, movement));

	lifeSpan_ -= GameTime::DeltaTime();
	if (lifeSpan_ <= 0.0f)
	{
		KillMe();
	}
}

void Bullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}

void Bullet::OnCollision(GameObject* pOther)
{
	if (pOther->GetName() == "Player")
	{
		pOther->KillMe();
		KillMe();
	}
}

void Bullet::SetParam(DirectX::XMFLOAT3 position, DirectX::XMVECTOR dir)
{
	transform_.position = position;
	dir_ = dir;
}
