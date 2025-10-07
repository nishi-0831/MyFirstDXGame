#include <wincodec.h>
#include "Texture.h"
#include <DirectXTex.h>
#include <filesystem>

namespace fs = std::filesystem;
//#include "Engine/Direct3D.h"
//DirectXTexのライブラリをリンク

#pragma comment(lib,"DirectXTex.lib")

//using namespace DirectX;

Texture::Texture()
{
	pSampler_ = nullptr;
	pSRV_ = nullptr;
}

Texture::~Texture()
{
}

HRESULT Texture::Load(std::string fileName)
{
	

	HRESULT hr;

	//実際に読んでゆくぅ　　　　　 
	std::wstring wfileName{fileName.begin(), fileName.end()};
	return Load(wfileName.c_str());
}

HRESULT Texture::Load(const wchar_t* fileName)
{
	TexMetadata metadata; //画像の付属情報

	ScratchImage image;   //画像本体

	HRESULT hr = LoadFromWICFile(fileName, WIC_FLAGS::WIC_FLAGS_NONE,
		&metadata, image);



	if (FAILED(hr))
	{

		return S_FALSE;
	}

	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	//画像の縮小などに線形補間使うよ
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//座標が0~1を超えたらどうするのか
	//CLAMPだと0～1に制限するよ
	//WRAPは繰り返し
	//MIRRORは反転する 1超えたら0に、0超えたら1に
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);

	if (FAILED(hr))
	{

		return S_FALSE;
	}
	//シェーダーリソースビュー

	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};

	//RGBA8ビットずつ
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//2Dテクスチャ
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	//テクスチャ版のLOD
	//1枚だけミップマップを用意しとくよ
	srv.Texture2D.MipLevels = 1;



	hr = CreateShaderResourceView(Direct3D::pDevice,

		image.GetImages(), image.GetImageCount(), metadata, &pSRV_);
	if (FAILED(hr))
	{

		return S_FALSE;
	}
	return S_OK;
}




void Texture::Release()
{
	SAFE_RELEASE(pSampler_);
	SAFE_RELEASE(pSRV_);
}
