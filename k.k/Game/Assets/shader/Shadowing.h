//シャドウイング

#ifndef _SHADOWING_H_
#define _SHADOWING_H_

#include "Shadowing_const.h"

static const int INFINITY = 40.0f;

///////////////////////////////////////
// サンプラステート。
///////////////////////////////////////
#include "Sampler.h"


// チェビシェフの不等式を利用して、影になる可能性を計算する。
float Chebyshev(float2 moments, float depth)
{
    if (depth <= moments.x) {
        return 0.0;
    }
    // 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
    float depth_sq = moments.x * moments.x;
    // このグループの分散具合を求める
    // 分散が大きいほど、varianceの数値は大きくなる
    float variance = moments.y-depth_sq;
    //このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める 
    float md  = depth - moments.x;
    // 光が届く確率を求める
    float lig_factor = variance / (variance + md * md);
    float lig_factor_min = 0.3f;
    // 光が届く確率の下限以下は影になるようにする。
    lig_factor = saturate((lig_factor - lig_factor_min) / (1.0f - lig_factor_min));
    // 光が届く確率から影になる確率を求める。影の濃さ
    return (1.0f - lig_factor);
}



float CalcShadowRate(
    Texture2D<float4> shadowMap[NUM_SHADOW_MAP],
    float4x4 mlvp[NUM_SHADOW_MAP],
    float3 worldPos,
    int isSoftShadow
)
{
    float shadow = 0.0f;

    for (int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++)
    {
        //ライトビュースクリーン空間の座標を計算する
        float4 posInLVP = mul( mlvp[cascadeIndex], float4( worldPos, 1.0f ));
        //Zの値を見て、このピクセルがこのシャドウマップに含まれているか判定
        float2 shadowMapUV=posInLVP.xy/posInLVP.w;
        float zInLVP=posInLVP.z/posInLVP.w;
        shadowMapUV *= float2(0.5f, -0.5f);
        shadowMapUV += 0.5f;
        //シャドウマップUVが範囲内か判定
        if(shadowMapUV.x>=0.0f&&shadowMapUV.x<=1.0f&&
        shadowMapUV.y>=0.0f&&shadowMapUV.y<=1.0f&&
        zInLVP<0.98f&&zInLVP>0.02f)
        {
            // シャドウマップから値をサンプリング
            float4 shadowValue = shadowMap[cascadeIndex].Sample(
                g_sampler, shadowMapUV
            );
            zInLVP -= 0.001f;
            //深度値
            float pos = exp(INFINITY * zInLVP);
            //影になる可能性を計算する
            shadow = Chebyshev(shadowValue.xy, pos);
            //ハードシャドウ
            /* if(zInLVP >= shadowValue.r)
            {
                shadow=0.5f;

            }*/
            //影を薄くする
            shadow *= 0.5f;
            break;
        }

    }

    return shadow;
}



#endif // !_SHADOWING_H_
