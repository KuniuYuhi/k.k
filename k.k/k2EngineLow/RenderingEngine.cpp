#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	void RenderingEngine::Init()
	{
		
		InitZPrepassRenderTarget();
		InitRenderTargets();
		m_shadow.Init();
		m_postEffect.Init(m_mainRenderTarget);
		InitCopyToFrameBufferSprite();



		m_sceneLight.Init();
		SetToonTextureDDS();
	}

	void RenderingEngine::InitRenderTargets()
	{
		//���C�������_�����O�^�[�Q�b�g
		m_mainRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		//	�e�N�X�`����m_mainRenderTarget�̃J���[�o�b�t�@�[
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();

		//2D�̃V�F�[�_�[���w�肷��
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		//�X�v���C�g��������
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	//�g�D�[���e�N�X�`����ݒ肷��
	void RenderingEngine::SetToonTextureDDS()
	{
		const wchar_t* toontexture = L"Assets/shader/ToonTextrue/toontexture2.DDS";
		m_toontexture.InitFromDDSFile(toontexture);
	}

	//���f�����X�g�Ɋi�[����Ă��郂�f����`�悷��
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderModel(rc);
		}
	}

	//���f�����X�g�̊i�[����Ă��郂�f���̃V���h�E�}�b�v�`��p�̃��f����`�悷��
	void RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderShadowModel(rc, camera);
		}
	}

	//�X�v���C�g���X�g�Ɋi�[����Ă���X�v���C�g��`�悷��
	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_spriteList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}

	//�t�H���g���X�g�Ɋi�[����Ă���t�H���g��`�悷��
	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_fontList)
		{
			fontObj->OnRenderFont(rc);
		}
	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_zprepassModelList) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
	}

	void RenderingEngine::InitZPrepassRenderTarget()
	{
		float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//���_�̈ʒu��ݒ肷��
		SetEyePos(g_camera3D->GetPosition());

		//�V���h�E�}�b�v�`��p�̃��f����`��
		m_shadow.Render(rc);

		SetmLVP(g_renderingEngine->GetLightCamera().GetViewProjectionMatrix());

		ZPrepass(rc);

		//�����_�����O�^�[�Q�b�g��m_mainRenderTarget�ɕύX����
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//�����_�����O�^�[�Q�b�g�ƃr���[�|�[�g��ݒ肷��
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//���f����`��
		ModelRendering(rc);

		//�����_�����O�^�[�Q�b�g�������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
		//�P�x���o�ƃK�E�V�A���u���[���s
		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z����
		m_postEffect.Render(rc, m_mainRenderTarget);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);

		//�X�v���C�g��`��
		SpriteRendering(rc);
		//�t�H���g��`��
		FontRendering(rc);

		//m_shadow.ShadowSpriteRender(rc);

		m_modelList.clear();
		m_spriteList.clear();
		m_fontList.clear();
		m_zprepassModelList.clear();
	}

}
