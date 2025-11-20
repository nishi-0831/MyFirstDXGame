#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

//コンスタントバッファ
struct CONSTANT_SPRITE_BUFFER
{
	//World,View,Projection変換行列
	DirectX::XMMATRIX matW;
	DirectX::XMMATRIX matVP;
};

//頂点情報
struct SPRITE_VERTEX
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR uv;
};

class Sprite
{
private:
	ID3D11Buffer* pVertexBuffer_;//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;//コンスタントバッファ

	Texture* pTexture_;
public:
	Transform* pTransform_;
	SPRITE_VERTEX vertices_[4];
	Sprite();
	~Sprite();
	HRESULT Initialze();
	void Draw();
	void Draw(DirectX::XMMATRIX& worldMatrix);
	HRESULT Load(std::string fileName);
	HRESULT Load(const wchar_t* fileName);

	void Release();
};

