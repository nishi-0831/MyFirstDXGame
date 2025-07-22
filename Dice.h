#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
class Quad;
struct light
{
	XMMATRIX lightRotMat;
};
class Dice
{
	//Quad��6��
	//center��p��,radius�Ŕz�u
	std::vector<Quad*> quads_;

	ID3D11Buffer* pConstantBuffer_;
public:
	HRESULT Initialze();
	void Draw();
	float radius_;
	XMVECTOR pos_;
	XMVECTOR rot_;
	XMVECTOR lightRot;
};

