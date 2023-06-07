/*!
 * @brief	シンプルなモデルシェーダー。
 */
////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal    : NORMAL;
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal        : NORMAL;			//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos			:TEXCOORD1;		//ワールド座標
};

//ディレクションライト構造体
struct DirectionLight
{
	float3 direction; //ライトの方向
	float3 color;     //ライトの色
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ディレクションライトの定数バッファー
cbuffer LightCB:register(b1){
	DirectionLight directionLight;      //ディレクションライト

	float3 eyepos;			//視点の位置
}


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4>g_normalMap : register(t1);			//法線マップ
Texture2D<float4>g_specularMap : register(t2);			//スペキュラマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection,float3 lightColor,float3 worldPos,float3 normal,float2 uv);
/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.worldPos = psIn.pos;
	//頂点法線をピクセルシェーダーに渡す
	psIn.normal=mul(m,vsIn.normal);



	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	
	//ランバート拡散反射
	float3 Lambart=CalcLambertDiffuse(
		directionLight.direction,directionLight.color,psIn.normal);

	//鏡面反射
	float3 Phong=CalcPhongSpecular(
					directionLight.direction,directionLight.color,psIn.worldPos,psIn.normal,psIn.uv);
	float3 lig=Lambart+Phong;

	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	albedoColor.xyz*=lig;


	return albedoColor;
}

//////////////////////////////////////////////////////////////////////////////////
//Lambert拡散反射を計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算し、ライトの影響度を求める
	float t=dot(normal,lightDirection);
	//内積の結果の-1をかける
	t*=-1.0f;
	//内積の結果が0以下なら0にする
	if(t<0.0f)
	{
		t=0.0f;
	}
	//ライトの影響度を返す
	return lightColor*t;
}

//////////////////////////////////////////////////////////////////////////////////
//phong鏡面反射を計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcPhongSpecular(float3 lightDirection,float3 lightColor,float3 worldPos,float3 normal,float2 uv)
{
	//反射ベクトルを求める
	float3 refVec=reflect(directionLight.direction,normal);
	//光が当たったサーフェイス(表面)から視点に伸びるベクトルを求める
	float3 toEye=eyepos-worldPos;
	toEye=normalize(toEye);

	//鏡面反射の強さを求める
	float t=dot(refVec,toEye);

	//鏡面反射の強さを0~1にする
	t=max(0.0f,t);

	//鏡面反射の強さを絞る
	t=pow(t,5.0f);

	//鏡面反射光
	float specularLig= lightColor*t;

	//スペキュラマップからスペキュラ反射の強さをサンプリング
	float specPower=g_specularMap.Sample(g_sampler,uv).r;
	specularLig*=specPower*1.2f;

	return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//ディレクションライトを計算
//////////////////////////////////////////////////////////////////////////////////
//float3 CalcLigFromDrectionLight(SPSIn psIn,float3 normal)
//{
//	float3 diffDirection=CalcLambertDiffuse(direction)
//
//	return;
//}
