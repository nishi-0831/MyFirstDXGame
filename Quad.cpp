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
	//頂点情報
	XMVECTOR vertices[] =
	{
		//四角形の頂点

		//左上
		XMVectorSet(-1.0f,1.0f,0.0f,0.0f),
		//右上
		XMVectorSet(1.0f,1.0f,0.0f,0.0f),
		//右下
		XMVectorSet(1.0f,-1.0f,0.0f,0.0f),
		//左下
		XMVectorSet(-1.0f,-1.0f,0.0f,0.0f),
	};

	//頂点バッファ設定
	D3D11_BUFFER_DESC bd_vertex;
	//頂点バッファとなるverticesのサイズを渡す
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;

	//頂点データ
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	if (FAILED(result))
	{
		MessageBox(nullptr, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
		return result;
	}

	//インデックスバッファ設定

	//インデックス情報
	int index[] = { 0,2,3, 0,1,2 };

	//インデックスバッファ作成
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
		//ウィンドウのハンドル、本文、タイトル、OKボタンの表示
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return result;
	}
#else
	//Create(Direct3D::pDevice->CreateBuffer(&bd_index, &data_index, &pIndexBuffer_));
#endif
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	//コンスタントバッファに入れたいのはWVP行列で、カメラの位置とかはゲーム中変わるかもだからこの時点ではなんも入れないよ
	result =Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(result))
	{
		MessageBox(nullptr, L"コンスタントバッファの作成に失敗しました", L"エラー", MB_OK);
		return result;
	}

	return S_OK;
}

void Quad::Draw()
{
	
#if 0
	//コンスタントバッファに渡す情報
	//カメラの位置
	XMVECTOR position = { 0,3,-10,0 };
	//注視点
	XMVECTOR target = { 0,0,0,0 };
	//上方向
	//ゲームによっては上方向が固定とは限らない
	XMVECTOR upVector = { 0,1,0,0 };

	//ビュー行列
	XMMATRIX view = XMMatrixLookAtLH(position, target, upVector);
	
	//視野角,アスペクト比、前方クリッピング面の深度、後方クリッピング面の深度
	//π(3.14...)をdegにしたら180度
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列
	
#endif
	XMMATRIX transMat = XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
	
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(0, 30, 0);
	XMMATRIX scaleMat = XMMatrixScaling(1, 1, 1);

	//ワールド行列は拡縮→回転→移動の順
	XMMATRIX worldMat = scaleMat * rotMat * transMat;

	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose( worldMat * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());

	//GPUからのデータアクセスを止める
	//CPUからデータ渡すからGPUに待ってもらう
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);

	//データを渡す
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));

	//書き込み終わったよってする
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	///頂点バッファをインプットアセンブラにセット
	UINT stride = sizeof(XMVECTOR);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファもセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファを頂点シェーダ、ピクセルシェーダにセット
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);

	
	//描画
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
