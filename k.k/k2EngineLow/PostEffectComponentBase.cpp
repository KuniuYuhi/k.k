#include "k2EngineLowPreCompile.h"
#include "PostEffectComponentBase.h"

void nsK2EngineLow::PostEffectComponentBase::Init(
	RenderTarget& mainRenderTarget, 
	RenderTarget& zprepassRenderTarget)
{
	OnInit(mainRenderTarget, zprepassRenderTarget);

	//�|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɃR�s�[����Ȃ�
	if (IsCopyResultTextureToMainRenderTarget()) {
		SpriteInitData initData;
		initData.m_width = mainRenderTarget.GetWidth();
		initData.m_height = mainRenderTarget.GetHeight();
		initData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
		initData.m_fxFilePath = "Assets/shader/Sprite.fx";
		initData.m_textures[0] = &GetResultTexture();
		m_copyMainRtSprite.Init(initData);
	}
}

void nsK2EngineLow::PostEffectComponentBase::Render(
	RenderContext& rc, RenderTarget& mainRenderTarget)
{
	//�|�X�g�G�t�F�N�g�������Ȃ珈�����Ȃ�
	if (!m_isEnable) {
		return;
	}
	OnRender(rc, mainRenderTarget);

	if (IsCopyResultTextureToMainRenderTarget()) {
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(mainRenderTarget);
		// �|�X�g�G�t�F�N�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɔ��f�B
		m_copyMainRtSprite.Draw(rc);
		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}
}
