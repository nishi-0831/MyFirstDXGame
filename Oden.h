#pragma once
#include "Engine/Transform.h"
#include "Engine/Fbx.h"
class Oden
{
public:
	void Initialize();
	Transform* pTransform_;
	Fbx* pFbx_;
	void Draw();
};
