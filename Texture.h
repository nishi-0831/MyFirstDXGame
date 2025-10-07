#pragma once
//#include <d3d11.h>
#include <string>
#include "Direct3D.h"

class Texture
{
private:
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
public:
	Texture();
	~Texture();
	HRESULT Load(std::string fileName);
	HRESULT Load(const wchar_t* fileName);
	void Release();

	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
	ID3D11SamplerState* GetSampler() { return pSampler_; }
};