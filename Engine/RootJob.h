#pragma once
#include "GameObject.h"
#include "../PlayScene.h"

class RootJob : public GameObject
{
public:
	RootJob() = delete;
	RootJob(GameObject* pParent);
	~RootJob();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};
