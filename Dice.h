#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
class Quad;
class Dice
{
	//Quad‚ð6‚Â
	//center‚ð—pˆÓ,radius‚Å”z’u
	std::vector<Quad*> quads_;
public:
	HRESULT Initialze();
	void Draw();
	float radius_;
	XMVECTOR pos_;
	XMVECTOR rot_;
};

