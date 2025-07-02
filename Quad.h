#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
//コンスタントバッファ
struct CONSTANT_BUFFER
{
	//World,View,Projection変換行列
	XMMATRIX matWVP;
};

class Quad
{
private:
	ID3D11Buffer* pVertexBuffer_;//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;//コンスタントバッファ
public:
	Quad();
	~Quad();
	HRESULT Initialze();
	void Draw();
	void Draw(XMMATRIX& worldMatrix);
	void Release();

	XMFLOAT3 pos_;
};

