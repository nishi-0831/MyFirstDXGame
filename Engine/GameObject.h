#pragma once

#include <string>
#include <list>
#include "Transform.h"
#include <type_traits>
class GameObject
{
protected:
	// 子供のオブジェクト
	std::list<GameObject*> childList_;
	Transform	transform_;
	GameObject*	pParent_;
	std::string	objectName_;
	bool isDead_;
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
	void UpdateSub();
	virtual void Draw() = 0;
	void DrawSub();

	void KillMe() { isDead_ = true; }
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetPosition(float x,float y,float z);
	template<typename T>
	GameObject* Instantiate(GameObject* parent);
	virtual void Release() = 0;
	void ReleaseSub();
};

template<typename T>
inline GameObject* GameObject::Instantiate(GameObject* parent)
{
	static_assert(std::is_base_of_v<GameObject,T> , "GameObjectを継承している必要があります");
	T* obj = new T(parent);
	obj->Initialize();
	childList_.push_back(obj);
	return obj;
}
