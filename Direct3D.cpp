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
			//LPWSTR ch2{ L"エラーメッセージが取得できませんでした"};
			const wchar_t* ch2{ L"エラーメッセージが取得できませんでした"};
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

		///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
		DXGI_SWAP_CHAIN_DESC scDesc;

		//とりあえず全部0
		ZeroMemory(&scDesc, sizeof(scDesc));

		//描画先のフォーマット
		scDesc.BufferDesc.Width = screenWidth;		//画面幅
		scDesc.BufferDesc.Height = screenHeight;	//画面高さ
		scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

		//FPS（1/60秒に1回）
		scDesc.BufferDesc.RefreshRate.Numerator = 60;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;

		//その他
		scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
		scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
		scDesc.BufferCount = 1;			//バックバッファの枚数
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
		scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
		scDesc.SampleDesc.Quality = 0;		//　〃

		////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
		D3D_FEATURE_LEVEL level;
		result = D3D11CreateDeviceAndSwapChain(
			nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
			D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
			nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
			0,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
			nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
			0,					// 上の引数でレベルを何個指定したか
			D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
			&scDesc,				// 上でいろいろ設定した構造体
			&pSwapChain,				// 無事完成したSwapChainのアドレスが返ってくる
			&pDevice,				// 無事完成したDeviceアドレスが返ってくる
			&level,					// 無事完成したDevice、Contextのレベルが返ってくる
			&pContext);				// 無事完成したContextのアドレスが返ってくる


		if (FAILED(result))
		{
			return result;
		}

			///////////////////////////レンダーターゲットビュー作成///////////////////////////////
			//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
			ID3D11Texture2D* pBackBuffer;
		result = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(result))
		{
			return result;
		}
		//レンダーターゲットビューを作成
		result =pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
		if (FAILED(result))
		{
			return result;
		}

		//一時的にバックバッファを取得しただけなので解放
		SAFE_RELEASE(pBackBuffer);

		///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
		D3D11_VIEWPORT vp;
		vp.Width = (float)screenWidth;	//幅
		vp.Height = (float)screenHeight;//高さ
		vp.MinDepth = 0.0f;	//手前
		vp.MaxDepth = 1.0f;	//奥
		vp.TopLeftX = 0;	//左
		vp.TopLeftY = 0;	//上

		//データを画面に描画するための一通りの設定（パイプライン）
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);            // 描画先を設定
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
		//頂点シェーダ作成(コンパイル)
		ID3DBlob* pCompileVS = nullptr;
		result = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
		if (FAILED(result))
		{
			MessageBox(nullptr, L"頂点シェーダのコンパイルに失敗しました", L"エラー", MB_OK);
			return result;
		}
		//assert(SUCCEEDED(result));
		
		result = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
		
		
		//assert(SUCCEEDED(result));
		if (FAILED(result))
		{
			MessageBox(nullptr, L"頂点シェーダの作成に失敗しました", L"エラー", MB_OK);
			return result;
		}

		//ピクセルシェーダ
		ID3DBlob* pCompilePS = nullptr;
		result = D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
		
		
		if (FAILED(result))
		{
			MessageBox(nullptr, L"ピクセルシェーダのコンパイルに失敗しました", L"エラー", MB_OK);
			return result;
		}
		result = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
		
		if (FAILED(result))
		{
			if (FAILED(result))
			{
				MessageBox(nullptr, L"ピクセルシェーダの作成に失敗しました", L"エラー", MB_OK);
				return result;
			}
		}
		//_wassert(GetMsg(result), _CRT_WIDE(__FILE__), (unsigned int)__LINE__);
			
			
			
		
		//assert(SUCCEEDED(result));

		//頂点インプットのレイアウト設定
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			//現在は座標のみ
			//RGB各32bitずつ,96bit
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,sizeof(XMVECTOR),D3D11_INPUT_PER_VERTEX_DATA,0},
			//{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//法線
		};

		const int size = sizeof(layout) / sizeof(layout[0]);
		//頂点インプットレイアウト作成
		//第二引数の2はlayout[]の要素数、頂点の情報の数
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
		//表面だけ描画
		rdc.CullMode = D3D11_CULL_BACK;
		rdc.FillMode = D3D11_FILL_SOLID;
		//ワイヤーフレームもあるの??
		//rdc.FillMode = D3D11_FILL_WIREFRAME;
		
		//FrontCounterClockwiseはポリゴンのどっちの面を「表側」とみなすか。
		//ふつうは「頂点が時計回りに見える面」は表。
		//逆時計回りか否か
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
		//背景の色
		float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

		//画面をクリア
		pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	}


	void EndDraw()
	{
		//スワップ（バックバッファを表に表示する）
		pSwapChain->Present(0, 0);
	}

	void Direct3D::Draw()
	{
		


		//描画処理


		
	}
	
}