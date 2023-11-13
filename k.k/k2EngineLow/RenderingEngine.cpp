#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	void RenderingEngine::Init()
	{
		InitZPrepassRenderTarget();
		InitRenderTargets();
		InitGBuffer();
		m_sceneLight.Init();
		InitShadowMapRender();
		

		m_postEffect.Init(m_mainRenderTarget);

		InitCopyToFrameBufferSprite();
		
		InitDefferedLighting_Sprite();
		
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
		const wchar_t* toontexture = L"Assets/shader/ToonTextrue/lamp_glay.DDS";
		m_toontexture.InitFromDDSFile(toontexture);
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// �A���x�h�J���[���o�͗p�̃����_�����O�^�[�Q�b�g������������
		float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_gBuffer[enGBufferAlbedoDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
		// �@���o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_SNORM,
			DXGI_FORMAT_UNKNOWN
		);
		//// ���^���b�N�A�e�p�����[�^�A�X���[�X�o�͗p�̃����_�����O�^�[�Q�b�g������������    
		//m_gBuffer[enGBufferMetaricShadowSmooth].Create(
		//	frameBuffer_w,
		//	frameBuffer_h,
		//	1,
		//	1,
		//	DXGI_FORMAT_R8G8B8A8_UNORM,
		//	DXGI_FORMAT_UNKNOWN
		//);
		// ���[���h���W�̃����_�����O�^�[�Q�b�g
		/*m_gBuffer[enGBufferWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);*/

	}

	void RenderingEngine::InitDefferedLighting_Sprite()
	{
		//�f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		SpriteInitData spriteInitData;

		// ��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�[�̕��ƍ����Ɠ���
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		//�f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			spriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}

		spriteInitData.m_fxFilePath= "Assets/shader/DeferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &GetSceneLight();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSceneLight());

		//�������f�[�^���g���ăX�v���C�g���쐬
		m_diferredLightingSprite.Init(spriteInitData);
	}

	//���f�����X�g�Ɋi�[����Ă��郂�f����`�悷��
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnForwardRender(rc);
		}
	}

	//���f�����X�g�̊i�[����Ă��郂�f���̃V���h�E�}�b�v�`��p�̃��f����`�悷��
	void RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
	{
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnRenderShadowModel(rc, camera);
		}
	}

	//�X�v���C�g���X�g�Ɋi�[����Ă���X�v���C�g��`�悷��
	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_renderObjects)
		{
			spriteObj->OnRender2D(rc);
		}
	}

	//�t�H���g���X�g�Ɋi�[����Ă���t�H���g��`�悷��
	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_renderObjects)
		{
			fontObj->OnRender2D(rc);
		}
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("RenderToShadowMap");
		
		m_shadowMapRenders.Render(
			rc,
			GetLightCamera(),
			m_renderObjects
		);

		EndGPUEvent();
	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");
		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
		EndGPUEvent();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedoDepth],         // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
			&m_gBuffer[enGBufferNormal]             // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
			//& m_gBuffer[enGBufferWorldPos]              // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
		};

		//�܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		//�����_�[�^�[�Q�b�g��ݒ�
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);
		//�����_�[�^�[�Q�b�g���N���A
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);
		//
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnRenderToGBuffer(rc);
		}

		//�����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

		m_diferredLightingSprite.Draw(rc);

		EndGPUEvent();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");
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
		EndGPUEvent();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");
		//�X�v���C�g��`��
		SpriteRendering(rc);
		//�t�H���g��`��
		//FontRendering(rc);
		EndGPUEvent();
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

	void RenderingEngine::InitShadowMapRender()
	{
		//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g������
		m_shadowMapRenders.Init();

	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//�V���h�E�}�b�v�ւ̕`��
		//ZPrepass
		//GBuffer�ւ̕`��
		//�t�H���[�h�����_�����O�����C�e�B���O���s��
		//�|�X�g�G�t�F�N�g
		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[

		// �V�[�����C�g�̃f�[�^���R�s�[�B
		m_deferredLightingCB.m_light = m_sceneLight.GetSceneLight();

		//���_�̈ʒu��ݒ肷��	
		SetEyePos(g_camera3D->GetPosition());

		//�V���h�E�}�b�v�`��p�̃��f����`��
		RenderToShadowMap(rc);

		//���C�g�J�����̐ݒ�
		//LightCameraUpDate();
		
		//���C�g�r���[�v���W�F�N�V�����s���ݒ�
		SetmLVP();

		//ZPrepass���f����`��
		ZPrepass(rc);
		//Gbuffer
		RenderToGBuffer(rc);
		//�t�H���[�h�����_�����O
		ForwardRendering(rc);

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

		// �o�^����Ă���`��I�u�W�F�N�g���N���A
		m_renderObjects.clear();
	}

}
