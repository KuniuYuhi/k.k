#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow {

	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		InitRenderTarget(mainRenderTarget);
		InitLuminanceSprite(mainRenderTarget);
		InitGaussianBlur();
		InitFinalSprite(mainRenderTarget);
	}

	void Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		rc.ClearRenderTargetView(m_luminanceRenderTarget);

		//�P�x���o���s��
		m_luminanceSprite.Draw(rc);

		//�����_�����O�G���W���ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

		//�K�E�V�A���u���[�����s����
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ肷��
		rc.SetRenderTargetAndViewport(mainRenderTarget);

		//�ŏI����
		m_finalSprite.Draw(rc);

		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}

	void Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
	{
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),	//���C�������_�����O�^�[�Q�b�g�Ɠ���
			mainRenderTarget.GetHeight(),	//���C�������_�����O�^�[�Q�b�g�Ɠ���
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),	//���C�������_�����O�^�[�Q�b�g�Ɠ���
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
	{
		//���������쐬
		SpriteInitData luminanceSpriteInitData;
		//�P�x�E�o�p�̃V�F�[�_�[�̃t�@�C�����w�肷��
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/postEffect.fx";
		//���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//�X�v���C�g�̕��ƍ�����luminanceRenderTarget�Ɠ���
		luminanceSpriteInitData.m_width = m_luminanceRenderTarget.GetWidth();
		luminanceSpriteInitData.m_height = m_luminanceRenderTarget.GetHeight();

		//�e�N�X�`���̓��C�������_�[�^�[�Q�b�g�̃J���[�o�b�t�@�[
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//�������ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//�쐬�����������������ɃX�v���C�g������������
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
	{
		//����������ݒ肷��
		SpriteInitData finalSpriteInitData;
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

		//mainRenderTarget�Ƃ��Ȃ����ƍ���
		finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
		finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

		//�{�P�摜����������̂Ő�p�V�F�[�_�[���g�p����
		finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/postEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z����
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//�J���[�o�b�t�@�[�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�[
		finalSpriteInitData.m_colorBufferFormat[0]= DXGI_FORMAT_R32G32B32A32_FLOAT;

		//�X�v���C�g��������
		m_finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::InitGaussianBlur()
	{
		//[0]�͋P�x�e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		//[1]��[0]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		//[2]��[1]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		//[3]��[2]�̃e�N�X�`���ɃK�E�V�A���u���[��������
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}


}
