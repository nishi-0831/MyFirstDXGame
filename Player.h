#pragma once
#include "Engine/GameObject.h"

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
private:
	int hModel_{ -1 };
	Fbx* pFbx_{ nullptr };
};