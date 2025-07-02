#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
//�R���X�^���g�o�b�t�@
struct CONSTANT_BUFFER
{
	//World,View,Projection�ϊ��s��
	XMMATRIX matWVP;
};

class Quad
{
private:
	ID3D11Buffer* pVertexBuffer_;//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;//�R���X�^���g�o�b�t�@
public:
	Quad();
	~Quad();
	HRESULT Initialze();
	void Draw();
	void Draw(XMMATRIX& worldMatrix);
	void Release();

	XMFLOAT3 pos_;
};

