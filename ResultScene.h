#pragma once
#include "Engine\\GameObject.h"
#include "Engine\\Sprite.h"
class ResultScene : public GameObject
{
public:
	ResultScene() = delete;
	ResultScene(GameObject* pParent);
	~ResultScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	Sprite* pSprite_;
};
