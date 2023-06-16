#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

void nsK2EngineLow::SpriteRender::Init(const char* filePath, const float width, const float height, AlphaBlendMode alphaBlendMode)
{
	SpriteInitData spriteInitData;
	//DDS�t�@�C��(�摜�t�@�C��)�̃t�@�C���p�X���w�肷��
	spriteInitData.m_ddsFilePath[0] = filePath;
	//sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//sprite�̕��ƍ�����ݒ肷��
	spriteInitData.m_width= static_cast<UINT>(width);
	spriteInitData.m_height = static_cast<UINT>(height);

	//sprite�̃u�����h���[�h��ݒ�
	spriteInitData.m_alphaBlendMode = alphaBlendMode;

	//�萔�o�b�t�@�[�Ƃ��đ���
	spriteInitData.m_expandConstantBuffer = &m_spriteCB;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_spriteCB);


	//sprite�������I�u�W�F�N�g���g���āAsprite������������
	m_sprite.Init(spriteInitData);

	m_isInit = true;
}

void nsK2EngineLow::SpriteRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddSpriteList(this);
}
