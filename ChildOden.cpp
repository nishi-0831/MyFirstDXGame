#include "ChildOden.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
ChildOden::ChildOden(GameObject* parent)
	: GameObject(parent, "ChildOden")
{
}

ChildOden::~ChildOden()
{
}

void ChildOden::Initialize()
{
	hModel_ = Model::Load("oden.fbx");
	//transform_.position.x = 1.0f;
	transform_.scale.x = 0.2f;
	transform_.scale.y = 0.2f;
	transform_.scale.z = 0.2f;
}

void ChildOden::Update()
{
	transform_.rotate.y += 1.0f;
}

void ChildOden::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void ChildOden::Release()
{
}
