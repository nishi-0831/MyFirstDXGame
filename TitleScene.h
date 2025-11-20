#pragma once
#include "Engine\\GameObject.h"
#include "Engine\\Sprite.h"
class TitleScene : public GameObject
{
public:
	TitleScene() = delete;
	TitleScene(GameObject* pParent);
	~TitleScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	Sprite* pSprite_;
};