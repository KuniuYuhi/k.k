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
	float3 normal    : NORMAL;		//法線マップ
	float2 uv 		: TEXCOORD0;	//UV座標。
	float3 tangent 	: TANGENT;		//接ベクトル	
	float3 biNormal : BINORMAL;		//従ベクトル

	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal        : NORMAL;			//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 tangent 	: TANGENT;		//接ベクトル	
	float3 biNormal : BINORMAL;		//従ベクトル
	float3 worldPos		:TEXCOORD1;		//ワールド座標
	float3 normalInView :TEXCOORD2;		//カメラ空間の法線
	float4 posInProj : TEXCOORD3;		//スクリーン座標
	float3 depth : TEXCOORD4;
};

//ディレクションライト構造体
struct DirectionLight
{
	float3 direction; //ライトの方向
	float3 color;     //ライトの色
};

//ポイントライト構造体
struct PointLight
{
	float3 position;	//ライトの座標
	float3 color;		//ライトの色
	float3 range;		//xが影響範囲、yが影響率に累乗するパラメータ
	int isUse;			//使用中かのフラグ
};

//スポットライト構造体
struct SpotLight
{
	float3 position;	//座標
	float3 angle;		//照射角度
	float3 color;		//ライトの色
	float3 range;		//xが影響範囲、yが影響率に累乗するパラメータ
	int isUse;			//使用中フラグ
	float3 direction;	//照射方向
};

//半球ライト構造体
struct HemiSphereLight
{
	float3 groundColor;		//地面色
	float3 skyColor;		//天球色
	int isUse;				//使用フラグ
	float3 groundNormal;	//地面の法線
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

//ライトの定数バッファー
cbuffer LightCB:register(b1){
	DirectionLight directionLight;      //ディレクションライト
	PointLight pointLight;				//ポイントライト
	SpotLight spotLight;				//スポットライト
	HemiSphereLight hemiSphereLight;	//半球ライト
	float3 ambient;			//環境光
	float3 eyepos;			//視点の位置
}


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4>g_normalMap : register(t1);			//法線マップ
Texture2D<float4>g_specularMap : register(t2);			//スペキュラマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
Texture2D<float4>g_toonMap : register(t10);				//トゥーンマップ
Texture2D<float4>g_depthTexture : register(t12);		//深度テクスチャにアクセス
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection,float3 lightColor,float3 worldPos,float3 normal,float2 uv);
float3 CalcLigFromDrectionLight(SPSIn psIn,float3 normal);
float3 CalcLigFromPointLight(SPSIn psIn,float3 normal);
float3 CalcLigFromSpotLight(SPSIn psIn,float3 normal);
float3 CalcLigFromhemiSphereLight(SPSIn psIn);
float3 CalcNormalMap(SPSIn psIn);
float4 CalcToonMap(SPSIn psIn,float3 lightDirection);
float4 CalcOutLine(SPSIn psIn,float4 color);

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
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	//法線
	psIn.normal=mul(m,vsIn.normal);
	psIn.normal=normalize(psIn.normal);

	//カメラ空間の法線を求める
	psIn.normalInView=mul(mView,psIn.normal);

	//接ベクトルと従ベクトルをワールド空間に変換する
	psIn.tangent=normalize(mul(mWorld,vsIn.tangent));
	psIn.biNormal=normalize(mul(mWorld,vsIn.biNormal));

	psIn.uv = vsIn.uv;

	//頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
	psIn.posInProj=psIn.pos;
	psIn.posInProj/=psIn.posInProj.w;

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
	//法線マップを計算
	float3 normal=CalcNormalMap(psIn);

	//ディレクションライトによるライティングの計算
	float3 directionLig=CalcLigFromDrectionLight(psIn,normal);
	
	//ポイントライトによるライティングの計算
	float3 pointLig={0.0f,0.0f,0.0f};
	if(pointLight.isUse==true)
	{
		pointLig=CalcLigFromPointLight(psIn,normal);
	}

	//スポットライトによるライティングの計算
	float3 spotLig={0.0f,0.0f,0.0f};
	if(spotLight.isUse==true)
	{
		spotLig=CalcLigFromSpotLight(psIn,normal);
	}

	//半球ライトによるライティングの計算
	float3 hemiLig={0.0f,0.0f,0.0f};
	if(hemiSphereLight.isUse==true)
	{
		hemiLig=CalcLigFromhemiSphereLight(psIn);
	}

	//ディレクションライトと環境光をたす				
	float3 lig = directionLig + pointLig + spotLig + hemiLig + ambient;

	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	albedoColor.xyz*=lig;

	//輪郭線の計算
	albedoColor=CalcOutLine(psIn,albedoColor);


	return albedoColor;
}

//トゥーンマップのピクセルシェーダー
float4 PSToonMain(SPSIn psIn) : SV_Target0
{
	//法線マップを計算
	float3 normal=CalcNormalMap(psIn);

	//ディレクションライトによるライティングの計算
	float3 directionLig=CalcLigFromDrectionLight(psIn,normal);
	
	//ポイントライトによるライティングの計算
	float3 pointLig={0.0f,0.0f,0.0f};
	if(pointLight.isUse==true)
	{
		pointLig=CalcLigFromPointLight(psIn,normal);
	}

	//スポットライトによるライティングの計算
	float3 spotLig={0.0f,0.0f,0.0f};
	if(spotLight.isUse==true)
	{
		spotLig=CalcLigFromSpotLight(psIn,normal);
	}

	//半球ライトによるライティングの計算
	float3 hemiLig={0.0f,0.0f,0.0f};
	if(hemiSphereLight.isUse==true)
	{
		hemiLig=CalcLigFromhemiSphereLight(psIn);
	}

	//ディレクションライトと環境光をたす				
	float3 lig = directionLig + pointLig + spotLig + hemiLig + ambient;

	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	albedoColor.xyz*=lig;

	//トゥーンマップの計算。ディレクションライトのみ
	float4 Toon=CalcToonMap(psIn,directionLight.direction);

	albedoColor*=Toon;

	//輪郭線の計算
	albedoColor=CalcOutLine(psIn,albedoColor);


	return albedoColor;
}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSShadowMapMain(SPSIn psIn) : SV_Target0
{
    //シャドウマップにZ値を書き込む
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
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
	t=pow(t,10.0f);

	//鏡面反射光
	float specularLig= lightColor*t;

	//スペキュラマップからスペキュラ反射の強さをサンプリング
	float specPower=g_specularMap.Sample(g_sampler,uv).r;
	specularLig*=specPower*0.1f;

	return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//ディレクションライトを計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn,float3 normal)
{
	//拡散反射
	float3 diffDirection=CalcLambertDiffuse(
		directionLight.direction,directionLight.color,normal);
	//鏡面反射
	float3 specDirection=CalcPhongSpecular(
		directionLight.direction,directionLight.color,psIn.worldPos,normal,psIn.uv);

	//サーフェイスの法線と光の入射方向に依存するリムの強さを求める
	float power1=1.0f-max(0.0f,dot(directionLight.direction,normal));
	//サーフェイスの法線と視線の方向に依存するリムの強さを求める
	float power2=1.0f-max(0.0f,psIn.normalInView.z*-1.0f);
	//最終的なリムの強さを求める
	float limPower=power1*power2;
	//強さを指数関数的にする
	limPower=pow(limPower,1.2f);
	//リムライトのカラーを計算する
	float3 limColor=limPower*directionLight.color;

	//最終的な光
	return diffDirection+specDirection;//+limColor;
}

//////////////////////////////////////////////////////////////////////////////////////
///ポイントライトを計算
//////////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromPointLight(SPSIn psIn,float3 normal)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する
	float3 ligDir=psIn.worldPos-pointLight.position;
	//正規化
	ligDir=normalize(ligDir);

	//減衰なしのLambert拡散反射を計算する
	float3 diffPoint=CalcLambertDiffuse(
		ligDir,
		pointLight.color,
		normal
	);

	//減衰なしのPhong鏡面反射を計算する
	float3 specPoint=CalcPhongSpecular(
		ligDir,
		pointLight.color,
		psIn.worldPos,
		normal,
		psIn.uv
	);

	//距離による影響率を計算する
	//ポイントライトとの距離を計算する
	float distance=length(psIn.worldPos-pointLight.position);

	//影響率は距離に比例して小さくなっていく
	float affect=1.0f-1.0f/pointLight.range.x*distance;

	//影響率がマイナスにならないようにする
	if(affect<0.0f)
	{
		affect=0.0f;
	}

	//影響を指数関数的にする。
	affect=pow(affect,pointLight.range.y);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
	diffPoint*=affect;
	specPoint*=affect;

	return diffPoint+specPoint;
}

///////////////////////////////////////////////////////////////////////////////////////
///スポットライトを計算
///////////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromSpotLight(SPSIn psIn,float3 normal)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する
	float3 ligDir=psIn.worldPos-spotLight.position;
	//正規化
	ligDir=normalize(ligDir);

	//減衰なしのLambert拡散反射を計算する
	float3 diffSpotLight=CalcLambertDiffuse(
		ligDir,
		spotLight.color,
		normal
	);

	//減衰なしのPhong鏡面反射を計算する
	float3 specSpotLight=CalcPhongSpecular(
		ligDir,
		spotLight.color,
		psIn.worldPos,
		normal,
		psIn.uv
	);

	//距離による影響率を計算する
	//ポイントライトとの距離を計算する
	float distance=length(psIn.worldPos-spotLight.position);

	//影響率は距離に比例して小さくなっていく
	float affect=1.0f-1.0f/spotLight.range.x*distance;
	if(affect<0.0f)
	{
		affect=0.0f;
	}

	//影響の仕方を指数関数的にする
	affect=pow(affect,spotLight.range.y);
	//影響率を乗算して反射光を弱める
	diffSpotLight*=affect;
	specSpotLight*=affect;

	//入射光と射出方向の角度を求める
	//dot()を利用して、内積を求める
	float angle=dot(ligDir,spotLight.direction);
	//求めた値をacos()に渡して角度を求める
	angle=abs(acos(angle));

	//角度に比例して小さくなっていく影響率を求める
	affect=1.0f-1.0f/spotLight.angle*angle;
	if(affect<0.0f)
	{
		affect=0.0f;
	}
	affect=pow(affect,spotLight.range.y);
	diffSpotLight*=affect;
	specSpotLight*=affect;

	return diffSpotLight+specSpotLight;
}

//////////////////////////////////////////////////////////////////////////////////////////
///半球ライトを計算
//////////////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromhemiSphereLight(SPSIn psIn)
{
	//ピクセルの法線と地面の法線の内積を計算する
	float t=dot(psIn.normal,hemiSphereLight.groundNormal);
	//
	t=(t+1.0f)/2.0f;
	//線形補間
	float3 hemiLight=lerp(
		hemiSphereLight.groundColor,
		hemiSphereLight.skyColor,
		t
	);

	return hemiLight;
}

/////////////////////////////////////////////////////////////////////////////////
///法線マップを計算
/////////////////////////////////////////////////////////////////////////////////
float3 CalcNormalMap(SPSIn psIn)
{
	float3 normal=psIn.normal;
	//法線マップからタンジェントスペースの法線をサンプリングする
	float3 localNormal=g_normalMap.Sample(g_sampler,psIn.uv).xyz;
	//タンジェントスペースの法線を0~1の範囲から-1~1の範囲に復元する
	localNormal=(localNormal-0.5f)*2.0f;

	
	//タンジェントスペースの法線をワールドスペースに変換する
	normal=psIn.tangent*localNormal.x
		+psIn.biNormal*localNormal.y
		+normal*localNormal.z;
		
	return normal;
}

//////////////////////////////////////////////////////////////////////////////////
///トゥーンマップを計算
//////////////////////////////////////////////////////////////////////////////////
float4 CalcToonMap(SPSIn psIn,float3 lightDirection)
{
	//ハーフランバート拡散照明による
	float p=dot(psIn.normal*-1.0f,lightDirection.xyz);
	p=p*0.5f+0.5f;
	p*=p;

	float4 Color=g_toonMap.Sample(g_sampler,float2(p,0.0f));

	return Color;
}

///////////////////////////////////////////////////////////////////////////////////
///輪郭線を計算
///////////////////////////////////////////////////////////////////////////////////
float4 CalcOutLine(SPSIn psIn,float4 color)
{
	 // 正規化スクリーン座標系からUV座標系に変換する
    float2 uv = psIn.posInProj.xy * float2( 0.5f, -0.5f) + 0.5f;

	// 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] = {
        float2(           0.0f,  1.0f / 720.0f), //上
        float2(           0.0f, -1.0f / 720.0f), //下
        float2( 1.0f / 1280.0f,           0.0f), //右
        float2(-1.0f / 1280.0f,           0.0f), //左
        float2( 1.0f / 1280.0f,  1.0f / 720.0f), //右上
        float2(-1.0f / 1280.0f,  1.0f / 720.0f), //左上
        float2( 1.0f / 1280.0f, -1.0f / 720.0f), //右下
        float2(-1.0f / 1280.0f, -1.0f / 720.0f)  //左下
    };

	//このピクセルの深度値を取得
	float depth = g_depthTexture.Sample(g_sampler,uv).x;
	// 近傍8テクセルの深度値の平均値を計算する
	float depth2=0.0f;
	for(int i = 0;i < 8;i++)
	{
		depth2+=g_depthTexture.Sample(g_sampler,uv+uvOffset[i]).x;
	}
	depth2/=8.0f;

	//自身の深度値と近傍8テクセルの深度値の差を調べる
	if(abs(depth-depth2)>0.00005f)
	{
		// 深度値が結構違う場合はピクセルカラーを黒にする
        // ->これがエッジカラーとなる
		color=float4(0.0f,0.0f,0.0f,1.0f);
		return color;
	}


	return color;
}
