#pragma once

#include <string>
#include <list>
#include "Transform.h"

class GameObject
{
	// 子供のオブジェクト
	std::list<GameObject*> childList_;
	Transform	transform_;
	GameObject*	pParent_;
	std::string	objectName_;

public:
	GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	GameObject(GameObject* parent, const std::string& name);
	virtual ~GameObject();

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
};