#include "Character.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
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

	transform_.scale.x = 0.2f;
	transform_.scale.y = 0.2f;
	transform_.scale.z = 0.2f;
}

void Character::Update()
{
	transform_.rotate.y += 1.0f;

}

void Character::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Character::Release()
{
}
