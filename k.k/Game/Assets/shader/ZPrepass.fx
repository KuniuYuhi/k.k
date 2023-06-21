///////////////////////////////////////
// ZPrepass
///////////////////////////////////////


///////////////////////////////////////
// 構造体。
///////////////////////////////////////

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 depth : TEXCOORD0;       //深度値。xにはプロジェクション空間、yにはカメラ空間での正規化されたZ値、zにはカメラ空間でのZ値
};



///////////////////////////////////////
// 定数バッファ。
///////////////////////////////////////

#include "ModelVSCommon.h"

// モデル用の定数バッファー
//cbuffer ModelCb : register(b0)
//{
   // float4x4 mWorld;
   // float4x4 mView;
   // float4x4 mProj;
//};




///////////////////////////////////////
// 関数
///////////////////////////////////////


// モデル用の頂点シェーダーのエントリーポイント
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer)
{
    SPSIn psIn;
    psIn.pos = CalcVertexPositionInWorldSpace(vsIn.pos, mWorldLocal, isUsePreComputedVertexBuffer);
    psIn.pos = mul(mView, psIn.pos);        // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.depth.z = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);        // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.depth.x = psIn.pos.z / psIn.pos.w;
    psIn.depth.y = saturate( psIn.pos.w / 1000.0f );
    
    return psIn;
}

// モデル用の頂点シェーダーのエントリーポイント
//SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
//{
    //SPSIn psIn;

   // psIn.pos = mul(mWorld, vsIn.pos); // モデルの頂点をワールド座標系に変換
   // psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
   // psIn.depth.z = psIn.pos.z;
   // psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
   // psIn.depth.x = psIn.pos.z / psIn.pos.w;
   // psIn.depth.y = saturate( psIn.pos.w / 1000.0f );
    
   // return psIn;
//}

// モデル用のピクセルシェーダーのエントリーポイント
float4 PSMain(SPSIn psIn) : SV_Target0
{
    return float4( psIn.depth.x, psIn.depth.y, psIn.depth.z, 1.0f );
}
