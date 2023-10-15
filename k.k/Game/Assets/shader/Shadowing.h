//�V���h�E�C���O�֌W�̏���

#ifndef _SHADOWING_H_
#define _SHADOWING_H_

#include "Shadowing_const.h"

static const int INFINITY = 40.0f;

///////////////////////////////////////
// サンプラステート。
///////////////////////////////////////
#include "Sampler.h"

// �`�F�r�V�F�t�̕s�����𗘗p���āA�e�ɂȂ�\�����v�Z����B
float Chebyshev(float2 moments, float depth)
{
    if (depth <= moments.x) {
        return 0.0;
    }
    // �Օ�����Ă���Ȃ�A�`�F�r�V�F�t�̕s�����𗘗p���Č���������m�������߂�
    float depth_sq = moments.x * moments.x;
    //���̃O���[�v�̕��U������߂�
    //���U���傫���قǁAvariance�̒l���傫���Ȃ�
    float variance = min(max(moments.y - depth_sq, 0.0001f), 1.0f);
    //���̃s�N�Z���̃��C�g���猩���[�x�l�ƃV���h�E�}�b�v�̕��ς̐[�x�l�̍������߂�
    float md  = depth - moments.x;
    // �����͂��m�������߂�
    float lig_factor = variance / (variance + md * md);
    float lig_factor_min = 0.3f;
    //�����͂��m���̉����ȉ��͉e�ɂȂ�悤�ɂ���
    lig_factor = saturate((lig_factor - lig_factor_min) / (1.0f - lig_factor_min));
    //�����͂��m������e�ɂȂ�m�������߂�
    return 1.0f - lig_factor;
}



float CalcShadowRate(
    Texture2D<float4> shadowMap[NUM_SHADOW_MAP],
    float4x4 mlvp,
    float3 worldPos,
    int isSoftShadow
)
{
    float shadow = 0.0f;

    for (int cascadeIndex = 0; cascadeIndex < NUM_SHADOW_MAP; cascadeIndex++)
    {
        //ライトビュースクリーン空間の座標を計算する
        float4 posInLVP = mul( mlvp, float4( worldPos, 1.0f ));
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
                g_sampler,shadowMapUV
            );
            zInLVP -= 0.001f;
            if(zInLVP>=shadowValue.r)
            {
                shadow=0.5f;
            }
            /*float pos = exp(INFINITY * zInLVP);
            
            if(pos>=shadowValue.r)
            {
                shadow=0.5f;
            }*/
            break;
            //zInLVP -= 0.001f;
        }

    }

    return shadow;
}



#endif // !_SHADOWING_H_
