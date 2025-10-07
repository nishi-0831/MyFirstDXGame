#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

//#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
//�R���X�^���g�o�b�t�@
struct CONSTANT_SPRITE_BUFFER
{
	//World,View,Projection�ϊ��s��
	DirectX::XMMATRIX matWVP;
	DirectX::XMMATRIX matW;
};

//���_���
struct SPRITE_VERTEX
{
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR uv;
};

class Sprite
{
private:
	ID3D11Buffer* pVertexBuffer_;//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;//�R���X�^���g�o�b�t�@

	Texture* pTexture_;
public:
	Transform* pTransform_;
	SPRITE_VERTEX vertices_[4];
	Sprite();
	~Sprite();
	HRESULT Initialze();
	void Draw();
	void Release();
};

