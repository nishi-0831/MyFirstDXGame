#include "GameObject.h"

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

void GameObject::ReleaseSub()
{
	this->Release();
	for (auto child : childList_)
	{
		child->ReleaseSub();
	}
}
