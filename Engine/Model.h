#pragma once
#include "Fbx.h"
#include "Transform.h"
#include <string>
namespace Model
{
	struct ModelData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string fileName_;
	};

	int Load(std::string fileName);
	void SetTransform(int hModel, const Transform& transform);
	void Draw(int hModel);
	void Release();
}