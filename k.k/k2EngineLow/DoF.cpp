#include "k2EngineLowPreCompile.h"
#include "DoF.h"

void nsK2EngineLow::DoF::OnInit(
	RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
{
	//�e�탌���_�����O�^�[�Q�b�g�̏�����
	InitRenderTarget(mainRenderTarget);
	//�Z�p�`�u���[�����邽�߂̊e��X�v���C�g�̏�����
	InitBlurSprite(mainRenderTarget);
}

void nsK2EngineLow::DoF::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	if (m_isEnable == false) {
		return;
	}
	
	//2���̃����_�����O�^�[�Q�b�g��ݒ肵�ă��f����`�悷��
	RenderTarget* rts[] = {
		&mainRenderTarget,
		&m_depthRenderTarget
	};

	//��ʊE�[�x�p�̃����_�����O�^�[�Q�b�g�ɕύX
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTargets(2, rts);
	//�����_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetsAndViewport(2, rts);
	// �����_�����O�^�[�Q�b�g���N���A
	rc.ClearRenderTargetViews(2, rts);
	//���f����`��
	g_renderingEngine->ModelRendering(rc);
	//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTargets(2, rts);

	//�����A�Ίp���u���[��������
	RenderTarget* blurRts[] = {
		&m_rtVerticalBlur,
		&m_rtDiagonalBlur
	};

	rc.WaitUntilToPossibleSetRenderTargets(2, blurRts);
	rc.SetRenderTargetsAndViewport(2, blurRts);
	rc.ClearRenderTargetViews(2, blurRts);
	//�����u���[�ƑΊp���u���[�̃����_�����O�^�[�Q�b�g����������
	m_vertDIagonalBlurSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTargets(2, blurRts);

	//�Z�p�`�u���[��������
	rc.WaitUntilToPossibleSetRenderTarget(m_rtPhomboidBlur);
	rc.SetRenderTargetAndViewport(m_rtPhomboidBlur);
	m_phomboidBlurSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(m_rtPhomboidBlur);

	// �{�P�摜�Ɛ[�x�e�N�X�`���𗘗p���āA�{�P�摜��`������ł���
	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	// ���C�������_�����O�^�[�Q�b�g��ݒ�
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	// �X�v���C�g��`��
	m_combineBokeImageSprite.Draw(rc);
	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::DoF::Render(RenderContext& rc, RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
{
	///////////////////////////////////////////////////////////////////
	////���C�������_�����O�^�[�Q�b�g�̃{�P�摜���쐬
	//m_blur.ExecuteOnGPU(rc, 10);

	////�{�P�摜�Ɛ[�x�e�N�X�`���𗘗p���āA�{�P�摜��`������ł���
	////���C�������_�����O�^�[�Q�b�g��ݒ�
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);

	//m_combineBokeImageSprite.Draw(rc);

	////�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::DoF::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//�V�[���̃J������Ԃł�Z�l���������ރ����_�����O�^�[�Q�b�g���쐬
	float clearColor[4] = { 10000.0f,10000.0f,10000.0f,1.0f };
	m_depthRenderTarget.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	
	//�����u���[�������邽�߂̃����_�����O�^�[�Q�b�g
	m_rtVerticalBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);
	//�Ίp���u���[�������邽�߂̃����_�����O�^�[�Q�b�g
	m_rtDiagonalBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);
	//�Z�p�`�u���[�������邽�߂̃����_�����O�^�[�Q�b�g
	m_rtPhomboidBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);

}

void nsK2EngineLow::DoF::InitBlurSprite(RenderTarget& mainRenderTarget)
{
	//�����A�Ίp���u���[�������邽�߂̃X�v���C�g��������
	SpriteInitData vertDiagonalBlurSpriteInitData;
	vertDiagonalBlurSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	vertDiagonalBlurSpriteInitData.m_width = mainRenderTarget.GetWidth();
	vertDiagonalBlurSpriteInitData.m_height = mainRenderTarget.GetHeight();
	vertDiagonalBlurSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/hexaBlur.fx";
	//�����A�Ίp���u���[�p�̃s�N�Z���V�F�[�_�[���w�肷��
	vertDiagonalBlurSpriteInitData.m_psEntryPoinFunc = "PSVerticalDiagonalBlur";
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[1] = mainRenderTarget.GetColorBufferFormat();
	//�X�v���C�g�̏�����
	m_vertDIagonalBlurSprite.Init(vertDiagonalBlurSpriteInitData);

	//�Z�p�`�u���[�������邽�߂̃X�v���C�g��������
	SpriteInitData phomboidBlurSpriteInitData;
	phomboidBlurSpriteInitData.m_textures[0] = &m_rtVerticalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_textures[1] = &m_rtDiagonalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_width = mainRenderTarget.GetWidth();
	phomboidBlurSpriteInitData.m_height = mainRenderTarget.GetHeight();
	phomboidBlurSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/hexaBlur.fx";

	//�Z�p�`�u���[�p�̃s�N�Z���V�F�[�_�[���w�肷��
	phomboidBlurSpriteInitData.m_psEntryPoinFunc = "PSRhomboidBlur";
	phomboidBlurSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	m_phomboidBlurSprite.Init(phomboidBlurSpriteInitData);

	//�{�P�摜�����p�̃X�v���C�g������������
	SpriteInitData  combineBokeImageSpriteInitData;
	//�g�p����e�N�X�`����2��
	combineBokeImageSpriteInitData.m_textures[0] = &m_rtPhomboidBlur.GetRenderTargetTexture();
	combineBokeImageSpriteInitData.m_textures[1] = &m_depthRenderTarget.GetRenderTargetTexture();
	combineBokeImageSpriteInitData.m_width = mainRenderTarget.GetWidth();
	combineBokeImageSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�����p�̃V�F�[�_�[���w�肷��
	combineBokeImageSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/DepthOfFeild.fx";
	combineBokeImageSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//�����𗘗p���ă{�P�摜���A���t�@�u�����f�B���O����̂ŁA���������[�h�ɂ���
	combineBokeImageSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//�X�v���C�g�̏�����
	m_combineBokeImageSprite.Init(combineBokeImageSpriteInitData);

}
