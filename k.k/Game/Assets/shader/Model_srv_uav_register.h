//////////////////////////////////////////////////////////////////////////////
// �t�H���[�h�����_�����O�Ŏg�p����郂�f���V�F�[�_�[��SRV_UAV���W�X�^�ݒ�
//////////////////////////////////////////////////////////////////////////////

#ifndef _MODEL_SRV_UAV_REGISTER_
#define _MODEL_SRV_UAV_REGISTER_

#include "Shadowing_const.h"

Texture2D<float4> g_albedo : register(t0);		//�A���x�h�}�b�v
Texture2D<float4>g_normalMap : register(t1);	//�@���}�b�v
Texture2D<float4>g_specularMap : register(t2);	//�X�y�L�����}�b�v
Texture2D<float4> g_shadowMap[NUM_SHADOW_MAP] : register(t10);  //�V���h�E�}�b�v�B
Texture2D<float4>g_toonMap : register(t11);		//�g�D�[���}�b�v
Texture2D<float4>g_depthTexture : register(t12);//�[�x�e�N�X�`���ɃA�N�Z�X


#endif // !_MODEL_SRV_UAV_REGISTER_
