#include "Character.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"
Character::Character(GameObject* parent)
	: GameObject(parent,"ChildOden")
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
	transform_.position.x = -2.0f;
	transform_.scale.x = 0.2f;
	transform_.scale.y = 0.2f;
	transform_.scale.z = 0.2f;
}

void Character::Update()
{
	//transform_.rotate.y += 1.0f;
	if (Input::IsKey(DIK_RIGHT))
	{
		transform_.position.x += 0.01f;
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
