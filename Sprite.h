#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"

using namespace DirectX;

//#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
//�R���X�^���g�o�b�t�@
struct CONSTANT_SPRITE_BUFFER
{
	//World,View,Projection�ϊ��s��
	XMMATRIX matWVP;
	XMMATRIX matW;
};

//���_���
struct SPRITE_VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
};

class Sprite
{
private:
	ID3D11Buffer* pVertexBuffer_;//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;//�R���X�^���g�o�b�t�@

	Texture* pTexture_;
public:
	XMVECTOR pos_;
	XMVECTOR rot_;
	SPRITE_VERTEX vertices_[4];
	Sprite();
	~Sprite();
	HRESULT Initialze();
	HRESULT Initialze(int row, int column);
	void Draw();
	void Draw(XMMATRIX& worldMatrix, XMMATRIX& normalTransMatrix);
	void Release();
	float rotY_;
};

