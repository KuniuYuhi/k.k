#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH = 1024;	//�����_�����O�^�[�Q�b�g�̕�
	const int RENDER_TARGET_HEIGHT = 1024;	//�����_�����O�^�[�Q�b�g�̍���

	const float LIGHT_CAMERA_WIDTH = 600.0f;
	const float LIGHT_CAMERA_HEIGHT = 600.0f;

	const float LIGHT_CAMERA_ANGLE = 20.0f;	//���C�g�J�����̃A���O��
}

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
		//InitShadowSprite();
	}

	void Shadow::Render(RenderContext& rc)
	{
		UpDateLightCamera();

		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap);
		//�e���f����`��
		g_renderingEngine->ShadowModelRendering(rc, m_lightCamera);
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
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
		m_shadowMap.Create(
			ShadowConst::RENDER_TARGET_WIDTH,
			ShadowConst::RENDER_TARGET_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);
	}

	void Shadow::InitLightCamera()
	{
		//�e�`�ʗp�̃��C�g�J�����̏�����
		m_lightCamera.SetPosition(m_lightCameraPosition);
		//���C�g���Ƃ炷�ꏊ
		m_lightCamera.SetTarget(0, 0, 0);
		//X��������ɂ���
		m_lightCamera.SetUp(1, 0, 0);
		//��p
		m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

		//�e�������Ȃ��悤�ɂ��邽�߂ɃJ�����𕽍s���e�ɂ���
		/*m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);*/

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

