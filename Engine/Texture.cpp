#include <wincodec.h>
#include "Texture.h"
#include <DirectXTex.h>
#include <filesystem>

namespace fs = std::filesystem;
//#include "Engine/Direct3D.h"
//DirectXTex�̃��C�u�����������N

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

	//���ۂɓǂ�ł䂭���@�@�@�@�@ 
	std::wstring wfileName{fileName.begin(), fileName.end()};
	return Load(wfileName.c_str());
}

HRESULT Texture::Load(const wchar_t* fileName)
{
	TexMetadata metadata; //�摜�̕t�����

	ScratchImage image;   //�摜�{��

	HRESULT hr = LoadFromWICFile(fileName, WIC_FLAGS::WIC_FLAGS_NONE,
		&metadata, image);



	if (FAILED(hr))
	{

		return S_FALSE;
	}

	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	//�摜�̏k���Ȃǂɐ��`��Ԏg����
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//���W��0~1�𒴂�����ǂ�����̂�
	//CLAMP����0�`1�ɐ��������
	//WRAP�͌J��Ԃ�
	//MIRROR�͔��]���� 1��������0�ɁA0��������1��
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);

	if (FAILED(hr))
	{

		return S_FALSE;
	}
	//�V�F�[�_�[���\�[�X�r���[

	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};

	//RGBA8�r�b�g����
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//2D�e�N�X�`��
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	//�e�N�X�`���ł�LOD
	//1�������~�b�v�}�b�v��p�ӂ��Ƃ���
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
