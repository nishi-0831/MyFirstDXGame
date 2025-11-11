#include "Character.h"
#include "Engine/SceneManager.h"
#include "Engine/Fbx.h"
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Engine/Input.h"
#include "Bullet.h"
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
		transform_.position.x += 0.01f;
	}
	if (Input::IsKey(DIK_LEFT))
	{
		transform_.position.x -= 0.01f;
	}
	if (Input::IsKey(DIK_UP))
	{
		transform_.position.z += 0.01f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.position.z -= 0.01f;
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
	//hitList_.push_back(pOther->pCollider_);

		/*std::string output = std::format( "{} : {}\n", objectName_.c_str(), pOther->objectName_.c_str());
		OutputDebugStringA(output.c_str());*/
}

void Character::Shot()
{
	//Bullet* bullet = Instantiate<Bullet>(nullptr);
	SceneManager* sceneManager = dynamic_cast<SceneManager*>( FindObject("SceneManager"));
	Bullet* bullet = Instantiate<Bullet>(sceneManager->GetCurrentScene());
	bullet->SetParam(transform_.position, transform_.Forward());
}
