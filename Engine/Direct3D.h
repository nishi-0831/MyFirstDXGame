#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "ReleaseUtility.h"
//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

enum SHADER_TYPE
{
	SHADER_3D,//3D用シェーダー
	SHADER_2D,//2D用シェーダー
	SHADER_MAX//シェーダーの最大数
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

	//シェーダ準備
	HRESULT InitShader3D();
	HRESULT InitShader2D();

	void SetShader(SHADER_TYPE type);//シェーダーをセット
	
	LPCWSTR GetMsg(HRESULT hr);
	//描画開始
	void BeginDraw();
	//描画終了
	void EndDraw();
	void Draw();
	//解放
	void Release();
}
