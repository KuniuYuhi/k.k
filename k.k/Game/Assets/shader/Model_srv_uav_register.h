//////////////////////////////////////////////////////////////////////////////
// �t�H���[�h�����_�����O�Ŏg�p����郂�f���V�F�[�_�[��SRV_UAV���W�X�^�ݒ�
//////////////////////////////////////////////////////////////////////////////

#ifndef _MODEL_SRV_UAV_REGISTER_
#define _MODEL_SRV_UAV_REGISTER_

#include "Shadowing_const.h"

Texture2D<float4> g_albedo : register(t0);		//�A���x�h�}�b�v
Texture2D<float4>g_normalMap : register(t1);	//�@���}�b�v
Texture2D<float4>g_specularMap : register(t2);	//�X�y�L�����}�b�v
//Texture2D<float4> g_shadowMap[NUM_SHADOW_MAP] : register(t12);  //�V���h�E�}�b�v�B
Texture2D<float4> g_shadowMap_0 : register(t12);  //�V���h�E�}�b�v�B
Texture2D<float4> g_shadowMap_1 : register(t13);  //�V���h�E�}�b�v�B
Texture2D<float4> g_shadowMap_2 : register(t14);  //�V���h�E�}�b�v�B

Texture2D<float4>g_toonMap : register(t10);		//�g�D�[���}�b�v
Texture2D<float4>g_depthTexture : register(t11);//�[�x�e�N�X�`���ɃA�N�Z�X


#endif // !_MODEL_SRV_UAV_REGISTER_
