
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

//ディレクションライト構造体
struct DirectionLight
{
	float3 direction; //ライトの方向
	float3 color;     //ライトの色
};

//ポイントライト構造体
struct PointLight
{
	float3 position;	//ライトの座標
	float3 color;		//ライトの色
	float3 range;		//xが影響範囲、yが影響率に累乗するパラメータ
	int isUse;			//使用中かのフラグ
};

//スポットライト構造体
struct SpotLight
{
	float3 position;	//座標
	float3 angle;		//照射角度
	float3 color;		//ライトの色
	float3 range;		//xが影響範囲、yが影響率に累乗するパラメータ
	int isUse;			//使用中フラグ
	float3 direction;	//照射方向
};

//半球ライト構造体
struct HemiSphereLight
{
	float3 groundColor;		//地面色
	float3 skyColor;		//天球色
	int isUse;				//使用フラグ
	float3 groundNormal;	//地面の法線
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer cb : register(b0){
	 float4x4 mvp;
    float4 mulColor;
};

//ライトの定数バッファー
cbuffer LightCB : register(b1){
	DirectionLight directionLight;      //ディレクションライト
	PointLight pointLight;				//ポイントライト
	SpotLight spotLight;				//スポットライト
	HemiSphereLight hemiSphereLight;	//半球ライト
	float3 ambient;			//環境光
	float3 eyepos;			//視点の位置
}

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> albedoTexture : register(t0); // アルベド
Texture2D<float4> normalTexture : register(t1); // 法線
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
    //G-Bufferの内容を使ってライティング
    float4 albedo = albedoTexture.Sample(Sampler, In.uv);
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;

    // ライトを計算
    float3 lig = 0.0f;
    float t = max(0.0f, dot(normal, directionLight.direction) * -1.0f);
    lig = directionLight.color * t;
    float4 finalColor = albedo;
    finalColor.xyz *= lig;

    // スペキュラ強度を引っ張ってきて、スペキュラライトを計算する

    return finalColor;
}
