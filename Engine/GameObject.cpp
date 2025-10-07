#include "GameObject.h"

GameObject::GameObject()
	: pParent_{nullptr}
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	: pParent_{ parent }
	, objectName_{ name }
{
}

GameObject::~GameObject()
{
}
