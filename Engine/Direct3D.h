#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
//#define Create(hr) if(FAILED(hr)){_wassert(Direct3D::GetMsg(hr), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);}

using namespace DirectX;

enum SHADER_TYPE
{
	SHADER_3D,//3D�p�V�F�[�_�[
	SHADER_2D,//2D�p�V�F�[�_�[
	SHADER_MAX//�V�F�[�_�[�̍ő吔
};


namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern IDXGISwapChain* pSwapChain;
	extern ID3D11RenderTargetView* pRenderTargetView;
	extern ID3D11VertexShader* pVertexShader;
	extern ID3D11PixelShader* pPixelShader;
	extern ID3D11InputLayout* pVertexLayout;
	extern ID3D11RasterizerState* pRasterizerState;
	extern ID3D11Texture2D* pDepthStencil;
	extern ID3D11DepthStencilView* pDepthStencilView;
	extern ID3D11DepthStencilState* pDepthStencilState;

	HRESULT Initialize(HWND hWnd, int screenWidth, int screenHeight);

	//�V�F�[�_����
	HRESULT InitShader3D();
	HRESULT InitShader2D();

	void SetShader(SHADER_TYPE type);//�V�F�[�_�[���Z�b�g
	

	LPCWSTR GetMsg(HRESULT hr);
	//�`��J�n
	void BeginDraw();
	//�`��I��
	void EndDraw();
	void Draw();
	//���
	void Release();
}
