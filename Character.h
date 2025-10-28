#pragma once
#include "Engine/GameObject.h"

class Fbx;
class Character : public GameObject
{
public:
	Character(GameObject* parent);
	~Character();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int hModel_{ -1 };
};