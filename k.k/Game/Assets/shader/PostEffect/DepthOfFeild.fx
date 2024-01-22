/*!
 * @brief ��ʊE�[�x
 */

cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP�s��
    float4 mulColor; // ��Z�J���[
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
 * @brief ���_�V�F�[�_�[
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

// �{�P�摜�Ɛ[�x�e�N�X�`���ɃA�N�Z�X���邽�߂̕ϐ���ǉ�
Texture2D<float4> bokeTexture : register(t0);
Texture2D<float4> depthTexture : register(t1);

sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// �{�P�摜�������ݗp
/////////////////////////////////////////////////////////

float4 PSMain(PSInput In) : SV_Target0
{
    //�J������Ԃł̐[�x�l���T���v�����O

    float depth = depthTexture.Sample(Sampler, In.uv).z;
    //return float4(depth / 1500.00, depth / 1500.00, depth / 1500.00, 1);
    //�J������Ԃł̐[�x�l��800�ȉ��Ȃ�s�N�Z���L��
    //�{�P�摜���������܂Ȃ�
    clip(depth - 400.0f);

    //�{�P�摜���T���v�����O
    float4 boke = bokeTexture.Sample(Sampler, In.uv);

    //�[�x�l����s�����x���v�Z����
    //�[�x�l800����n�܂�A�[�x�l2000�ōő�̃{�P��ɂȂ�
    boke.a = min(1.0f, (depth - 400.0f) / 1000.0f);

    return boke;
}