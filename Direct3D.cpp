#include <d3dcompiler.h>
#include "Direct3D.h"
#include <assert.h>
#include <windows.h>
//#include <system_error>
#include <string>
#include <iostream>
#include <wchar.h>



namespace Direct3D
{

	LPCWSTR Direct3D::GetMsg(HRESULT hr)
	{
		LPWSTR messageBuffer = nullptr;
		
		size_t size = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&messageBuffer,
			0,
			NULL);
		if (size > 0 && messageBuffer != nullptr)
		{
			return messageBuffer;
		}
		else
		{
			//LPWSTR ch2{ L"�G���[���b�Z�[�W���擾�ł��܂���ł���"};
			const wchar_t* ch2{ L"�G���[���b�Z�[�W���擾�ł��܂���ł���"};
			return ch2;
			
		}
	}

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	ID3D11InputLayout* pVertexLayout = nullptr;
	ID3D11RasterizerState* pRasterizerState = nullptr;

	void Direct3D::Release()
	{
		SAFE_RELEASE(pRasterizerState);
		SAFE_RELEASE(pVertexLayout);
		SAFE_RELEASE(pPixelShader);
		SAFE_RELEASE(pVertexShader);
		SAFE_RELEASE(pRenderTargetView);
		SAFE_RELEASE(pSwapChain);
		SAFE_RELEASE(pContext);
		SAFE_RELEASE(pDevice);
	}
	HRESULT Direct3D::Initialize(HWND hWnd, int screenWidth, int screenHeight)
	{

		HRESULT result;

		///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
		DXGI_SWAP_CHAIN_DESC scDesc;

		//�Ƃ肠�����S��0
		ZeroMemory(&scDesc, sizeof(scDesc));

		//�`���̃t�H�[�}�b�g
		scDesc.BufferDesc.Width = screenWidth;		//��ʕ�
		scDesc.BufferDesc.Height = screenHeight;	//��ʍ���
		scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

		//FPS�i1/60�b��1��j
		scDesc.BufferDesc.RefreshRate.Numerator = 60;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;

		//���̑�
		scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
		scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
		scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
		scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
		scDesc.SampleDesc.Quality = 0;		//�@�V

		////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
		D3D_FEATURE_LEVEL level;
		result = D3D11CreateDeviceAndSwapChain(
			nullptr,				// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
			D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
			nullptr,				// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
			0,					// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
			nullptr,				// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
			0,					// ��̈����Ń��x�������w�肵����
			D3D11_SDK_VERSION,			// SDK�̃o�[�W�����B�K�����̒l
			&scDesc,				// ��ł��낢��ݒ肵���\����
			&pSwapChain,				// ������������SwapChain�̃A�h���X���Ԃ��Ă���
			&pDevice,				// ������������Device�A�h���X���Ԃ��Ă���
			&level,					// ������������Device�AContext�̃��x�����Ԃ��Ă���
			&pContext);				// ������������Context�̃A�h���X���Ԃ��Ă���


		if (FAILED(result))
		{
			return result;
		}

			///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
			//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
			ID3D11Texture2D* pBackBuffer;
		result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(result))
		{
			return result;
		}
		//�����_�[�^�[�Q�b�g�r���[���쐬
		result =pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
		if (FAILED(result))
		{
			return result;
		}

		//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
		SAFE_RELEASE(pBackBuffer);

		///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
		D3D11_VIEWPORT vp;
		vp.Width = (float)screenWidth;	//��
		vp.Height = (float)screenHeight;//����
		vp.MinDepth = 0.0f;	//��O
		vp.MaxDepth = 1.0f;	//��
		vp.TopLeftX = 0;	//��
		vp.TopLeftY = 0;	//��

		//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // �f�[�^�̓��͎�ނ��w��
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);            // �`����ݒ�
		pContext->RSSetViewports(1, &vp);

		
		result = InitShader();
		if (FAILED(result))
		{
			return result;
		}

		return S_OK;
	}

	HRESULT Direct3D::InitShader()
	{
		HRESULT result;
		//���_�V�F�[�_�쐬(�R���p�C��)
		ID3DBlob* pCompileVS = nullptr;
		result = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
		if (FAILED(result))
		{
			MessageBox(nullptr, L"���_�V�F�[�_�̃R���p�C���Ɏ��s���܂���", L"�G���[", MB_OK);
			return result;
		}
		//assert(SUCCEEDED(result));
		
		result = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
		
		
		//assert(SUCCEEDED(result));
		if (FAILED(result))
		{
			MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
			return result;
		}

		//�s�N�Z���V�F�[�_
		ID3DBlob* pCompilePS = nullptr;
		result = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
		
		
		if (FAILED(result))
		{
			MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̃R���p�C���Ɏ��s���܂���", L"�G���[", MB_OK);
			return result;
		}
		result = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
		
		if (FAILED(result))
		{
			if (FAILED(result))
			{
				MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
				return result;
			}
		}
		//_wassert(GetMsg(result), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);
			
			
			
		
		//assert(SUCCEEDED(result));

		//���_�C���v�b�g�̃��C�A�E�g�ݒ�
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//���݂͍��W�̂�
			//RGB�e32bit����,96bit
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,sizeof(XMVECTOR),D3D11_INPUT_PER_VERTEX_DATA,0},
			//{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//�@��
		};

		const int size = sizeof(layout) / sizeof(layout[0]);
		//���_�C���v�b�g���C�A�E�g�쐬
		//��������2��layout[]�̗v�f���A���_�̏��̐�
		result = pDevice->CreateInputLayout(layout, size, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
		if (FAILED(result))
		{
			if (FAILED(result))
			{
				return result;
			}
		}
		//assert(SUCCEEDED(result));
		
		
		
		
		D3D11_RASTERIZER_DESC rdc = {};
		//�\�ʂ����`��
		rdc.CullMode = D3D11_CULL_BACK;
		rdc.FillMode = D3D11_FILL_SOLID;
		//���C���[�t���[���������??
		//rdc.FillMode = D3D11_FILL_WIREFRAME;
		
		//FrontCounterClockwise�̓|���S���̂ǂ����̖ʂ��u�\���v�Ƃ݂Ȃ����B
		//�ӂ��́u���_�����v���Ɍ�����ʁv�͕\�B
		//�t���v��肩�ۂ�
		rdc.FrontCounterClockwise = FALSE;
		result = pDevice->CreateRasterizerState(&rdc, &pRasterizerState);
		if (FAILED(result))
		{
			if (FAILED(result))
			{
				return result;
			}
		}
		//assert(SUCCEEDED(result));
		
		
		pContext->VSSetShader(pVertexShader, NULL, 0);
		pContext->PSSetShader(pPixelShader, NULL, 0);
		pContext->IASetInputLayout(pVertexLayout);
		pContext->RSSetState(pRasterizerState);

		SAFE_RELEASE(pCompileVS);
		SAFE_RELEASE(pCompilePS);

		return S_OK;
	}

	void BeginDraw()
	{
		//�w�i�̐F
		float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

		//��ʂ��N���A
		pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	}


	void EndDraw()
	{
		//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
		pSwapChain->Present(0, 0);
	}

	void Direct3D::Draw()
	{
		


		//�`�揈��


		
	}
	
}