//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //ワールド行列
    float4x4 matNormalTrans;//法線をワールド座標系の方向ベクトルにする行列
    float4 diffuse;
    bool materialFlag;
    float padding[3];
};

cbuffer light
{
    float4x4 lightRot;
};


//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
                 //セマンティクス
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD;     //UV座標
    float4 color : COLOR; //色
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD,float4 normal : NORMAL )
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

    //法線を回転
    normal = mul(normal, matNormalTrans);
    normal.w = 0;
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;//UV座標はそのまま

    float4 light = float4(0, 0, -1, 0);
    light = normalize(-light);
    
    
    outData.color = dot(normalize(normal), -light);

	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //サンプラーでuv座標元にテクスチャを色々やってる
    //第一引数にサンプラー
    //第二引数にuv座標
    float4 color;
    if (materialFlag == true)
    {
        color = g_texture.Sample(g_sampler, inData.uv) * inData.color;
    }
    else
    {
        color = diffuse;
        //color = float4(1, 1, 1, 1);
    }
    return color;
        //return g_texture.Sample(g_sampler, inData.uv) * inData.color;
}