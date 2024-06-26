/*!
 * @brief 被写界深度
 */

cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP行列
    float4 mulColor; // 乗算カラー
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

// ボケ画像と深度テクスチャにアクセスするための変数を追加
Texture2D<float4> bokeTexture : register(t0);
Texture2D<float4> depthTexture : register(t1);

sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// ボケ画像書き込み用
/////////////////////////////////////////////////////////

float4 PSMain(PSInput In) : SV_Target0
{
    //カメラ空間での深度値をサンプリング

    float depth = depthTexture.Sample(Sampler, In.uv).z;
    //return float4(depth / 1500.00, depth / 1500.00, depth / 1500.00, 1);
    //カメラ空間での深度値が800以下ならピクセルキル
    //ボケ画像を書き込まない
    clip(depth - 400.0f);

    //ボケ画像をサンプリング
    float4 boke = bokeTexture.Sample(Sampler, In.uv);

    //深度値から不透明度を計算する
    //深度値800から始まり、深度値2000で最大のボケ具合になる
    boke.a = min(1.0f, (depth - 400.0f) / 1000.0f);

    return boke;
}