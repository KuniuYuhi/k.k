#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH = 6000;	//�����_�����O�^�[�Q�b�g�̕�
	const int RENDER_TARGET_HEIGHT = 6000;	//�����_�����O�^�[�Q�b�g�̍���

	const float LIGHT_CAMERA_WIDTH = 2000.0f;
	const float LIGHT_CAMERA_HEIGHT = 2000.0f;

	const float LIGHT_CAMERA_ANGLE = 20.0f;	//���C�g�J�����̃A���O��
}

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
	}

	//void Shadow::Render(RenderContext& rc)
	//{
	//	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	//	rc.SetRenderTargetAndViewport(m_shadowMap);
	//	rc.ClearRenderTargetView(m_shadowMap);
	//	int shadowMapNo = 0;
	//	//�e���f����`��
	//	g_renderingEngine->ShadowModelRendering(rc, m_lightCamera);
	//	//�������݊����҂�
	//	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
	//}

	void Shadow::Render(
		RenderContext& rc,
		int ligNo,
		Vector3& lightDirection,
		std::vector< IRenderer* >& renderObjects
	)
	{
		// ���C�g�̍ő�̍����������_���[��AABB����v�Z����B

		int shadowMapNo = 0;
		for (auto& shadowMap : m_shadowMaps) {
			rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
			rc.SetRenderTargetAndViewport(shadowMap);
			rc.ClearRenderTargetView(shadowMap);

			for (auto& renderer : renderObjects) {
				renderer->OnRenderShadowMap(
					rc,
					ligNo,
					shadowMapNo,
					m_lightCamera
				);
			}

			//�`�悪�I�������N���A
			//m_renderers.clear();

			// �������݊����҂�
			rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
			shadowMapNo++;
		}

		//�����_�����O�G���W���̕��ł��
		UpDateLightCamera();
	}

	void Shadow::ShadowSpriteRender(RenderContext& rc)
	{
		shadowSprite.Update(
			{ FRAME_BUFFER_W / -2.0f,FRAME_BUFFER_H / 2.0f,0.0f },
			g_quatIdentity,
			g_vec3One,
			{ 0.0f,1.0f }
		);
		shadowSprite.Draw(rc);
	}

	void Shadow::InitRenderTarget()
	{
		//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g
		/*m_shadowMap.Create(
			ShadowConst::RENDER_TARGET_WIDTH,
			ShadowConst::RENDER_TARGET_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);*/

		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		DXGI_FORMAT colorFormat;
		DXGI_FORMAT depthFormat;

		colorFormat = g_hardShadowMapFormat.colorBufferFormat;
		depthFormat = g_hardShadowMapFormat.depthBufferFormat;

		//�ߌi�p�̃V���h�E�}�b�v
		m_shadowMaps[0].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);

	}

	void Shadow::InitLightCamera()
	{
		//�e�`�ʗp�̃��C�g�J�����̏�����
		m_lightCamera.SetPosition(m_lightCameraPosition);
		//���C�g���Ƃ炷�ꏊ(�e�����Ƃ����ꏊ)
		m_lightCamera.SetTarget(0, 0, 0);
		//X��������ɂ���
		m_lightCamera.SetUp(1, 0, 0);
		//��p
		m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

		//�e�������Ȃ��悤�ɂ��邽�߂ɃJ�����𕽍s���e�ɂ���
		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);

		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		m_lightCamera.Update();

	}

	void Shadow::InitShadowSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_shadowMap.GetRenderTargetTexture();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_width = 512;
		spriteInitData.m_height = 512;

		shadowSprite.Init(spriteInitData);
	}

	void Shadow::UpDateLightCamera()
	{
		//���C�g�J�����̈ʒu���v�Z����
		Vector3 lightCamPos = g_camera3D->GetTarget();
		lightCamPos.x += m_lightCameraPosition.x;
		lightCamPos.y = m_lightCameraPosition.y;
		lightCamPos.z += m_lightCameraPosition.z;

		m_lightCamera.SetPosition(lightCamPos);
		m_lightCamera.SetTarget(g_camera3D->GetTarget());
		m_lightCamera.Update();
	}
	


}

