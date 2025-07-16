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
    float4x4 matNormalTrans;
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
    normal.w = 0;
    normal = mul(normal, matNormalTrans);
	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;

    float4 light = float4(0,0, 1, 0);
    light = normalize(-light);
    
    //outData.color = dot(normal, light);
    outData.color = dot(normalize(normal), -light);
    //float4 rgb = float4(255, 255, 255,0);
    //outData.color = normal * rgb;

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
    return g_texture.Sample(g_sampler, inData.uv) * inData.color;
}