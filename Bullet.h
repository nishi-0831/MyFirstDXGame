#pragma once
#include "Engine/GameObject.h"

class Fbx;

class Bullet : public GameObject
{
public:
	Bullet(GameObject* pParent);
	~Bullet();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pOther) override;
	void SetParam(DirectX::XMFLOAT3 position, DirectX::XMVECTOR dir);

private:
	int hModel_{ -1 };
	float speed_;
	float lifeSpan_;
	float currTime_;
	DirectX::XMVECTOR dir_;
};
