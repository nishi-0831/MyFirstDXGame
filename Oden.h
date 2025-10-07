#pragma once
#include "Transform.h"
#include "Fbx.h"
class Oden
{
public:
	void Initialize();
	Transform* pTransform_;
	Fbx* pFbx_;
	void Draw();
};
