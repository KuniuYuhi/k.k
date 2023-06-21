//���f���̒��_�V�F�[�_�[�֌W�̋��ʃw�b�_�[



///////////////////////////////////////
// �萔�o�b�t�@�B
///////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;          //���_���W�B
    float3 normal : NORMAL;         //�@���B
    float2 uv : TEXCOORD0;          //UV���W�B
    float3 tangent : TANGENT;      //�ڃx�N�g���B
    float3 biNormal : BINORMAL;     //�]�x�N�g���B
    int4  Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
StructuredBuffer<float4x4> g_boneMatrix         : register(t3);	    //�{�[���s��B
StructuredBuffer<float4x4> g_worldMatrixArray   : register(t10);	//���[���h�s��̔z��B�C���X�^���V���O�`��̍ۂɗL���B

///////////////////////////////////////
// �֐��錾
///////////////////////////////////////
SPSIn VSMainCore(SVSIn vsIn, float4x4 mWorldLocal, uniform bool isUsePreComputedVertexBuffer);

////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
/// <summary>
//�X�L���s����v�Z����B
/// </summary>
float4x4 CalcSkinMatrix(SVSIn skinVert)
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
/// ���[���h��Ԃ̒��_���W���v�Z����B
/// </summary>
/// <param name="vertexPos">���_���W</param>
/// <param name="mWorld">���[���h�s��</param>
/// <param name="isUsePreComputedVertexBuffer">���O�v�Z�ς݂̒��_�o�b�t�@�𗘗p���Ă���H</param>
float4 CalcVertexPositionInWorldSpace(float4 vertexPos, float4x4 mWorld, uniform bool isUsePreComputedVertexBuffer)
{
    float4 pos;
    if (isUsePreComputedVertexBuffer) {
        pos = vertexPos;
    }
    else {
        pos = mul(mWorld, vertexPos);  // ���f���̒��_�����[���h���W�n�ɕϊ�
    }

    return pos;
}
/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, mWorld, false);
}
/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐�(�C���X�^���V���O�`��p)�B
/// </summary>
SPSIn VSMainInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
    return VSMainCore(vsIn, g_worldMatrixArray[instanceID], false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMainSkin(SVSIn vsIn)
{
    return VSMainCore(vsIn, CalcSkinMatrix(vsIn), false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐�(�C���X�^���V���O�`��p�B
/// </summary>
SPSIn VSMainSkinInstancing(SVSIn vsIn, uint instanceID : SV_InstanceID)
{
    float4x4 mWorldLocal = CalcSkinMatrix(vsIn);
    mWorldLocal = mul(g_worldMatrixArray[instanceID], mWorldLocal);
    return VSMainCore(vsIn, mWorldLocal, false);
}
/// <summary>
/// ���O�v�Z�ς݂̒��_�o�b�t�@���g�����_�V�F�[�_�[�̃G���g���[�֐��B
/// �X�L�����b�V���p
/// </summary>
SPSIn VSMainSkinUsePreComputedVertexBuffer(SVSIn vsIn)
{
    return VSMainCore(vsIn, (float4x4)0, true);
}
/// <summary>
/// ���O�v�Z�ς݂̒��_�o�b�t�@���g�����_�V�F�[�_�[�̃G���g���[�֐��B
/// �X�L���Ȃ����b�V���p
/// </summary>
SPSIn VSMainUsePreComputedVertexBuffer(SVSIn vsIn)
{
    return VSMainCore(vsIn, (float4x4)0, true);
}
/// <summary>
/// ���[���h�X�y�[�X�̖@���A�ڃx�N�g���A�]�x�N�g�����v�Z����B
/// </summary>
/// <param name="oNormal">�@���̏o�͐�</param>
/// <param name="oTangent">�ڃx�N�g���̏o�͐�</param>
/// <param name="oBiNormal">�]�x�N�g���̏o�͐�</param>
/// <param name="mWorld">���[���h�s��</param>
/// <param name="iNormal">�@��</param>
/// <param name="iTanget">�ڃx�N�g��</param>
/// <param name="iBiNormal">�]�x�N�g��</param>
/// <param name="isUsePreComputedVertexBuffer">���O�v�Z�ςݒ��_�o�b�t�@�𗘗p����H/param>
void CalcVertexNormalTangentBiNormalInWorldSpace(
    out float3 oNormal,
    out float3 oTangent,
    out float3 oBiNormal,
    float4x4 mWorld,
    float3 iNormal,
    float3 iTangent,
    float3 iBiNormal,
    uniform bool isUsePreComputedVertexBuffer
)
{
    if (isUsePreComputedVertexBuffer) {
        // ���O�v�Z�ςݒ��_�o�b�t�@�𗘗p����B
        oNormal = iNormal;
        oTangent = iTangent;
        oBiNormal = iBiNormal;
    }
    else {
        // 
        float3x3 m3x3 = (float3x3)mWorld;
        oNormal = normalize(mul(m3x3, iNormal));
        oTangent = normalize(mul(m3x3, iTangent));
        oBiNormal = normalize(mul(m3x3, iBiNormal));
    }
}