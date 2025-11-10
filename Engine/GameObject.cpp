#include "GameObject.h"
#include "Direct3D.h"
#include "SphereCollider.h"
#include <format>
GameObject::GameObject()
	: pParent_{nullptr}
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	: pParent_{ parent }
	, objectName_{ name }
{
	if (parent != nullptr)
	{
		transform_.pParent_ = &(parent->transform_);
	}
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	this->Update();


	RoundRobin(GetRootJob());
	for (auto child : childList_)
	{
		child->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->isDead_)
		{
			(*itr)->ReleaseSub();
			delete (*itr);
			itr = childList_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void GameObject::DrawSub()
{
	this->Draw();
	for (auto child : childList_)
	{
		child->DrawSub();
	}
}

void GameObject::SetPosition(DirectX::XMFLOAT3 position)
{
	transform_.position = position;
}

void GameObject::SetPosition(float x, float y, float z)
{
	transform_.position = DirectX::XMFLOAT3{ x,y,z };
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
	{
		return this;
	}
	else
	{
		return pParent_->GetRootJob();
	}
}

GameObject* GameObject::FindChildObject(const std::string& name)
{
	if (objectName_ == name)
	{
		return this;
	}
	else
	{
		// 自身の子供を調べていく
		for (auto child : childList_)
		{
			GameObject* result = child->FindChildObject(name);
			if (result != nullptr)
			{
				return result;
			}
		}

		// 自身の子供にいなかった
		return nullptr;
	}
}

GameObject* GameObject::FindObject(const std::string& name)
{
	GameObject* pRootJob = GetRootJob();
	
	GameObject* result = pRootJob->FindChildObject(name);

	return result;
}

GameObject* GameObject::GetParent()
{
	return pParent_;
}

void GameObject::AddCollider(SphereCollider* pCollider)
{
	pCollider_ = pCollider;
}

void GameObject::Collision(GameObject* pOther)
{
	if (pCollider_ == nullptr)
		return;
	if (pOther->pCollider_ == nullptr)
		return;
	if (pCollider_ == pOther->pCollider_)
		return;

	if (pCollider_->IsHit(*pOther->pCollider_))
	{
		//hitList_.push_back(pOther->pCollider_);
		std::string output = std::format( "{} : {}\n", objectName_.c_str(), pOther->objectName_.c_str());
		OutputDebugStringA(output.c_str());
	}
}

void GameObject::RoundRobin(GameObject* pOther)
{
	// 自分にコライダーなかったら return
	/*if (pCollider_ == nullptr)
		return;*/

	// 自分とターゲット自体の当たり判定
	Collision(pOther);

	// 再帰的な奴
	for (GameObject* child : pOther->childList_)
	{
		RoundRobin(child);
	}
}

void GameObject::ReleaseSub()
{
	this->Release();
	for (auto child : childList_)
	{
		child->ReleaseSub();
		SAFE_DELETE(child);
	}
}
