#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
#define Create(hr) if(FAILED(hr)){_wassert(Direct3D::GetMsg(hr), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);}

using namespace DirectX;

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
	HRESULT Initialize(HWND hWnd, int screenWidth, int screenHeight);

	//シェーダ準備
	HRESULT InitShader();

	LPCWSTR GetMsg(HRESULT hr);
	//描画開始
	void BeginDraw();
	//描画終了
	void EndDraw();
	void Draw();
	//解放
	void Release();
}
