#include "Quad.h"
#include "Camera.h"

Quad::Quad()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	pos_ = XMFLOAT3(0, 0, 0);
}

Quad::~Quad()
{
}

HRESULT Quad::Initialze()
{
	HRESULT result = S_FALSE;

	//pos_ = XMVectorSet(0, 3, 10, 0);
	pos_ = XMFLOAT3(0, 3, 10);
	//���_���
	XMVECTOR vertices[] =
	{
		//�l�p�`�̒��_

		//����
		XMVectorSet(-1.0f,1.0f,0.0f,0.0f),
		//�E��
		XMVectorSet(1.0f,1.0f,0.0f,0.0f),
		//�E��
		XMVectorSet(1.0f,-1.0f,0.0f,0.0f),
		//����
		XMVectorSet(-1.0f,-1.0f,0.0f,0.0f),
	};

	//���_�o�b�t�@�ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	//���_�o�b�t�@�ƂȂ�vertices�̃T�C�Y��n��
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	//���_�f�[�^
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	if (FAILED(result))
	{
		MessageBox(nullptr, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return result;
	}

	//�C���f�b�N�X�o�b�t�@�ݒ�

	//�C���f�b�N�X���
	int index[] = { 0,2,3, 0,1,2 };

	//�C���f�b�N�X�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd_index;
	bd_index.Usage = D3D11_USAGE_DEFAULT;
	bd_index.ByteWidth = sizeof(index);
	bd_index.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd_index.CPUAccessFlags = 0;
	bd_index.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data_index;
	data_index.pSysMem = index;
	data_index.SysMemPitch = 0;
	data_index.SysMemSlicePitch = 0;
#if 1
	result = Direct3D::pDevice->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_);
	if (FAILED(result))
	{
		//�E�B���h�E�̃n���h���A�{���A�^�C�g���AOK�{�^���̕\��
		MessageBox(nullptr, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return result;
	}
#else
	//Create(Direct3D::pDevice->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_));
#endif
	//�R���X�^���g�o�b�t�@�쐬
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	//�R���X�^���g�o�b�t�@�ɓ��ꂽ���̂�WVP�s��ŁA�J�����̈ʒu�Ƃ��̓Q�[�����ς�邩�������炱�̎��_�ł͂Ȃ������Ȃ���
	result =Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return result;
	}

	return S_OK;
}

void Quad::Draw()
{
	
#if 0
	//�R���X�^���g�o�b�t�@�ɓn�����
	//�J�����̈ʒu
	XMVECTOR position = { 0,3,-10,0 };
	//�����_
	XMVECTOR target = { 0,0,0,0 };
	//�����
	//�Q�[���ɂ���Ă͏�������Œ�Ƃ͌���Ȃ�
	XMVECTOR upVector = { 0,1,0,0 };

	//�r���[�s��
	XMMATRIX view = XMMatrixLookAtLH(position, target, upVector);
	
	//����p,�A�X�y�N�g��A�O���N���b�s���O�ʂ̐[�x�A����N���b�s���O�ʂ̐[�x
	//��(3.14...)��deg�ɂ�����180�x
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//�ˉe�s��
	
#endif
	XMMATRIX transMat = XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
	
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(0, 30, 0);
	XMMATRIX scaleMat = XMMatrixScaling(1, 1, 1);

	//���[���h�s��͊g�k����]���ړ��̏�
	XMMATRIX worldMat = scaleMat * rotMat * transMat;

	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose( worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());

	//GPU����̃f�[�^�A�N�Z�X���~�߂�
	//CPU����f�[�^�n������GPU�ɑ҂��Ă��炤
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);

	//�f�[�^��n��
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

	//�������ݏI���������Ă���
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	///���_�o�b�t�@���C���v�b�g�A�Z���u���ɃZ�b�g
	UINT stride = sizeof(XMVECTOR);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@�𒸓_�V�F�[�_�A�s�N�Z���V�F�[�_�ɃZ�b�g
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

	
	//�`��
	Direct3D::pContext->DrawIndexed(6, 0, 0);
}

void Quad::Draw(XMMATRIX& worldMatrix)
{

}

void Quad::Release()
{
	SAFE_RELEASE(pVertexBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pConstantBuffer_);
}
