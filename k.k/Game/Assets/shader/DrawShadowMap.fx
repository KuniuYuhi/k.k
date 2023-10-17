/*!
 * @brief シャドウマップ描画用のシェーダー
 */

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;   // スクリーン空間でのピクセルの座標
    float2 depth : TEXCOORD1;   // ライト空間での深度情報
};

static const int INFINITY = 40.0f; 

///////////////////////////////////////
// 頂点シェーダーの共通処理をインクルードする。
///////////////////////////////////////

#include "ModelVSCommon.h"

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;

    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    psIn.depth = 0.0f;
    
    return psIn;
}
/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //シャドウマップにZ値を書き込む
    //return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);

    float depth = psIn.pos.z ;
    float pos = exp(INFINITY * depth);
    return float4(pos, pos*pos, 0.0f, 1.0f);
}