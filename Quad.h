#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"

using namespace DirectX;

//#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
//コンスタントバッファ
struct CONSTANT_BUFFER
{
	//World,View,Projection変換行列
	XMMATRIX matWVP;
	XMMATRIX matW;
	XMMATRIX matNormalTrans;
};

//頂点情報
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};

class Quad
{
private:
	ID3D11Buffer* pVertexBuffer_;//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;//コンスタントバッファ

	Texture* pTexture_;
public:
	XMVECTOR pos_;
	XMVECTOR rot_;
	VERTEX vertices_[4];
	Quad();
	~Quad();
	HRESULT Initialze();
	HRESULT Initialze(int row,int column);
	HRESULT Initialze(int row,int column,int dir);
	HRESULT Initialze(VERTEX* vertices);
	void Draw();
	void Draw(XMMATRIX& worldMatrix, XMMATRIX& normalTransMatrix);
	void Release();
	float rotY_;
};

