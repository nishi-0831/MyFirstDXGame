#pragma once

#include <string>
#include <list>
#include "Transform.h"
#include <type_traits>
#include "SphereCollider.h"

class GameObject
{
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
	void UpdateSub();
	void DrawSub();

	void KillMe() { isDead_ = true; }
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetPosition(float x,float y,float z);
	template<typename T>
	T* Instantiate(GameObject* parent);

	GameObject* GetRootJob();
	GameObject* FindChildObject(const std::string& name);
	GameObject* FindObject(const std::string& name);
	
	GameObject* GetParent();
	
	void AddCollider(SphereCollider* pCollider);
	/// <summary>
	/// 他のゲームオブジェクト一つと当たり判定を行う
	/// </summary>
	/// <param name="pOther"></param>
	void Collision(GameObject* pOther);
	/// <summary>
	/// 総当たりで当たり判定を行う
	/// </summary>
	/// <param name="pOther"></param>
	void RoundRobin(GameObject* pOther);
	virtual void OnCollision(GameObject* pOther) {};

	virtual void Release() = 0;
	void ReleaseSub();
	std::string GetName() { return objectName_; }
	Transform	transform_;
protected:
	// 子供のオブジェクト
	std::list<GameObject*> childList_;
	GameObject*	pParent_;
	std::string	objectName_;
	bool isDead_;
	SphereCollider* pCollider_;
	std::list<SphereCollider*> hitList_;
};

template<typename T>
inline T* GameObject::Instantiate(GameObject* parent)
{
	static_assert(std::is_base_of_v<GameObject,T> , "GameObjectを継承している必要があります");
	T* obj{ nullptr };
	if (parent == nullptr)
	{
		obj = new T(GetRootJob());
	}
	else
	{
		obj = new T(parent);
	}

	obj->Initialize();
	if (parent != nullptr)
	{
		parent->childList_.push_back(obj);
	}
	return obj;
}


