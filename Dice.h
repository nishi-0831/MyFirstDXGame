#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
class Quad;
class Dice
{
	//Quadを6つ
	//centerを用意,radiusで配置
	std::vector<Quad*> quads_;
public:
	HRESULT Initialze();
	void Draw();
	float radius_;
	XMVECTOR pos_;
	XMVECTOR rot_;
};

