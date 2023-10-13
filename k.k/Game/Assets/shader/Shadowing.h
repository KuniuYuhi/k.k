//シャドウイング関係の処理

#ifndef _SHADOWING_H_
#define _SHADOWING_H_

#include "Shadowing_const.h"

static const int INFINITY = 40.0f;

///////////////////////////////////////
// サンプラステート
///////////////////////////////////////
sampler Sampler : register(s0);	// s0レジスタに関連付けされているサンプラステート

//float CalcShadowRate(
//	Texture2D<float4> shadowMap[NUM_SHADW_MAP][NUM_SHADW_MAP];
//)



#endif // !_SHADOWING_H_
