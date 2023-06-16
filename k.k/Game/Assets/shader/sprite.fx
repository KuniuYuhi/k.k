/*!
 * @brief	スプライト用のシェーダー
 */

cbuffer cb : register(b0){
	float4x4 mvp;		
	float4 mulColor;	
};

//スプライトの定数バッファー
cbuffer SpriteCB : register(b1){
	float2 wipeDir;		//ワイプ方向
	bool grayScale;		//グレースケールするかどうか
	bool simpleWipe;	//単純なリニアワイプをするかどうか
	float wipeSize;		//ワイプサイズ
}

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
void SimpleWipe(PSInput psin);

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

	//PSInput psIn;

	if(grayScale)
	{
		return CalcGrayScale(color);
	}
	//これにすると画像が元に戻る
	//if(simpleWipe)
	SimpleWipe(In);
	

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
void SimpleWipe(PSInput psin)
{
	clip(psin.pos.x-wipeSize);
}