#pragma once
#include <DirectXMath.h>


//�ʒu�A�����A�g�嗦�Ȃǂ��Ǘ�����N���X
class Transform
{
	DirectX::XMMATRIX matTranslate_;	//�ړ��s��
	DirectX::XMMATRIX matRotate_;	//��]�s��	
	DirectX::XMMATRIX matScale_;	//�g��s��

public:
	DirectX::XMFLOAT3 position;	//�ʒu
	DirectX::XMFLOAT3 rotate;	//����
	DirectX::XMFLOAT3 scale;	//�g�嗦
	Transform* pParent_; //�e

	//�R���X�g���N�^
	Transform();

	//�f�X�g���N�^
	~Transform();

	//�e�s��̌v�Z
	void Calculation();

	//���[���h�s����擾
	DirectX::XMMATRIX GetWorldMatrix();

	//�@���ϊ��p�̍s��
	DirectX::XMMATRIX GetNormalMatrix();
};