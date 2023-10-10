/*!
 * @brief	シンプルなモデルシェーダー。
 */
////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////

//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal        : NORMAL;			//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 tangent 	: TANGENT;		//接ベクトル	
	float3 biNormal : BINORMAL;		//従ベクトル
	//float3 worldPos		:TEXCOORD1;		//ワールド座標
	//float3 normalInView :TEXCOORD2;		//カメラ空間の法線
	//float4 posInProj : TEXCOORD3;		//スクリーン座標
	//float3 depth : TEXCOORD4;
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float4 albedo : SV_Target0;         // アルベド
    float4 normal : SV_Target1;         // 法線
    //float4 metaricShadowSmooth : SV_Target2;  // メタリック、影パラメータ、スムース。rにメタリック、gに影パラメータ、aにスムース。
};

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////

#include "ModelVSCommon.h"

///////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      //アルベドマップ
Texture2D<float4> g_normal : register(t1);      //法線マップ

///////////////////////////////////////
// サンプラーステート
///////////////////////////////////////
sampler g_sampler : register(s0);

///////////////////////////////////////
// 関数
///////////////////////////////////////

// 法線マップから法線を取得。
float3 GetNormalFromNormalMap(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = g_normal.SampleLevel (g_sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;
    
    return newNormal;
}

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換

    // ワールド空間の法線、接ベクトル、従ベクトルを計算する。
    CalcVertexNormalTangentBiNormalInWorldSpace(
		psIn.normal,
		psIn.tangent,
		psIn.biNormal,
		mWorldLocal,
		vsIn.normal,
		vsIn.tangent,
		vsIn.biNormal,
		isUsePreComputedVertexBuffer
	);

    psIn.uv = vsIn.uv;
    
    return psIn;
}

SPSOut PSMainCore(SPSIn psIn,int isShadowReciever)
{
	//GBufferに出力
	SPSOut psOut;
	//アルベドカラーと深度値を出力
	psOut.albedo=g_albedo.Sample(g_sampler,psIn.uv);
	clip(psOut.albedo.a - 0.2f);	//ピクセルキル
	//アルベドカラーのαを深度値にする
	psOut.albedo.w = psIn.pos.z;

	// 法線を出力
    // 出力は0～1に丸められてしまうのでマイナスの値が失われてしまう
    // なので-1～1を0～1に変換する
    // (-1 ～ 1) ÷ 2.0       = (-0.5 ～ 0.5)
    // (-0.5 ～ 0.5) + 0.5  = (0.0 ～ 1.0)
	psOut.normal.xyz = GetNormalFromNormalMap(
		psIn.normal,psIn.tangent,psIn.biNormal,psIn.uv);
	psOut.normal.w = 1.0f;

	return psOut;
}

// モデル用のピクセルシェーダーのエントリーポイント
SPSOut PSMain(SPSIn psIn)
{
    return PSMainCore(psIn, 0);
}
SPSOut PSMainShadowReciever(SPSIn psIn)
{
    return PSMainCore(psIn, 1);
}
