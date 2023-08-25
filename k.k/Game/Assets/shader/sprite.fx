/*!
 * @brief	スプライト用のシェーダー
 */

cbuffer cb : register(b0){
	float4x4 mvp;		
	float4 mulColor;	
};

//スプライトの定数バッファー
cbuffer SpriteCB : register(b1){
	float2 RoundWipeStartPosition;   //	円形ワイプを行う始点
	bool grayScale;		//グレースケールするかどうか
	float2 wipeDirection;		//ワイプ方向
	bool simpleWipe;	//単純なリニアワイプをするかどうか
	float wipeSize;		//ワイプサイズ
	bool RoundWipe;	//円形ワイプを使用するかどうか
	bool WipeWithDirection;	//方向を指定するリニアワイプをするかどうか
};

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
///関数定義
/////////////////////////////////////////////////////////////
float4 CalcGrayScale(float4 color);
void CalcSimpleWipe(PSInput psin);
void CalcWipeWithDirection(PSInput psin);
void CalcRoundWipe(PSInput psIn);

/////////////////////////////////////////////////////////////
///変数定義
/////////////////////////////////////////////////////////////

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain( PSInput In ) : SV_Target0
{
	float4 color = colorTexture.Sample(Sampler, In.uv) * mulColor;

	if(grayScale)
	{
		return CalcGrayScale(color);
	}

	//これにすると画像が元に戻る
	if(simpleWipe)
	{
		CalcSimpleWipe(In);
	}
	
	if(WipeWithDirection)
	{
		CalcWipeWithDirection(In);
	}
	
	if(RoundWipe)
	{
		CalcRoundWipe(In);
	}
	

	return color;
}

//グレースケールの処理
float4 CalcGrayScale(float4 color)
{
	float Y = color.r * 0.29891f + color.g * 0.58661f + color.b * 0.11448f;
		color.r = Y;
		color.g = Y;
		color.b = Y;

	return color;
}

//単純なリニアワイプの計算
void CalcSimpleWipe(PSInput psin)
{
	clip(psin.pos.x-wipeSize);
}

//方向を指定するリニアワイプの計算
void CalcWipeWithDirection(PSInput psin)
{
	float t=dot(wipeDirection,psin.pos.xy);
	clip(t-wipeSize);
}

//円形ワイプの計算
void CalcRoundWipe(PSInput psIn)
{
	//始点からこのピクセルに向かって伸びるベクトルを計算する
	float2 posFromCenter=psIn.pos.xy-RoundWipeStartPosition;
	//ピクセルキル
	clip(length(posFromCenter)-wipeSize);
}