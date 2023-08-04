/*!
 * @brief	スカイキューブ用のシェーダー。
 */

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// 頂点シェーダーの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent  : TANGENT;
    float3 biNormal : BINORMAL;
};

//ピクセルシェーダーへの入力
struct SPSIn
{
	float4 pos : SV_POSITION;       //座標
	float3 normal : NORMAL;         //法線
	float3 tangent  : TANGENT;      //接ベクトル
	float3 biNormal : BINORMAL;     //従ベクトル
	float2 uv : TEXCOORD0;          //UV座標
	float3 worldPos : TEXCOORD1;    //ワールド座標
};
cbuffer SkyCubeCb : register(b1)
{
    float luminance;	// 輝度
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //ノーマルマップ
Texture2D<float4> g_spacular : register(t2);    //スペキュラマップ
TextureCube<float4> g_skyCubeMap : register(t10);
sampler g_sampler : register(s0);

// ノーマルマップから法線を計算
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normal.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.normal = mul(mWorld, vsIn.normal);
    psIn.normal = normalize(psIn.normal);
    psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
	psIn.uv = vsIn.uv;

    return psIn;
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor;
    float3 normal = normalize(psIn.normal);
    albedoColor = g_skyCubeMap.Sample(g_sampler,normal * -1.0f) * luminance;
    return albedoColor;
}