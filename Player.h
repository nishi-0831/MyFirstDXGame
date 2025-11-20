#pragma once
#include "Engine/GameObject.h"
#include "Event.h"
#include "ICommander.h"
#include <list>
class Fbx;
class Player : public GameObject
{
public:
	Player(GameObject* parent);
	~Player();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pOther) override;
	void AddObserver(IObserver* pObserver);
	void Notify(Event event);
private:
	std::list<IObserver*> observers_;
	int hModel_{ -1 };
	Fbx* pFbx_{ nullptr };
};