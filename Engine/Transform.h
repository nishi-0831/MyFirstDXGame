#pragma once
#include <DirectXMath.h>


//位置、向き、拡大率などを管理するクラス
class Transform
{
	DirectX::XMMATRIX matTranslate_;	//移動行列
	DirectX::XMMATRIX matRotate_;	//回転行列	
	DirectX::XMMATRIX matScale_;	//拡大行列

public:
	DirectX::XMFLOAT3 position;	//位置
	DirectX::XMFLOAT3 rotate;	//向き
	DirectX::XMFLOAT3 scale;	//拡大率
	Transform* pParent_; //親

	//コンストラクタ
	Transform();

	//デストラクタ
	~Transform();

	//各行列の計算
	void Calculation();

	//親の Transformを設定 
	void SetParent(Transform* pParent) { pParent_ = pParent; };

	DirectX::XMVECTOR Forward();

	//ワールド行列を取得
	DirectX::XMMATRIX GetWorldMatrix();

	DirectX::XMMATRIX GetWorldRotMatrix();
	//法線変換用の行列
	DirectX::XMMATRIX GetNormalMatrix();
};