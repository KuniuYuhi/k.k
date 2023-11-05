#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH = 6000;	//�����_�����O�^�[�Q�b�g�̕�
	const int RENDER_TARGET_HEIGHT = 6000;	//�����_�����O�^�[�Q�b�g�̍���

	const float LIGHT_CAMERA_WIDTH = 4000.0f;
	const float LIGHT_CAMERA_HEIGHT = 4000.0f;

	const float LIGHT_CAMERA_ANGLE = 20.0f;	//���C�g�J�����̃A���O��
}

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
	}

	void Shadow::Render(
		RenderContext& rc,
		Camera& lightCamera,
		std::vector< IRenderer* >& renderObjects
	)
	{
		// ���C�g�̍ő�̍����������_���[��AABB����v�Z����B
		m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(
			lightCamera,
			m_cascadeAreaRateArray
		);

		int shadowMapNo = 0;
		for (auto& shadowMap : m_shadowMaps) {
			rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
			rc.SetRenderTargetAndViewport(shadowMap);
			rc.ClearRenderTargetView(shadowMap);

			for (auto& renderer : renderObjects) {
				renderer->OnRenderShadowMap(
					rc,
					shadowMapNo,
					m_lightCamera,
					m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
				);
			}

			//�`�悪�I�������N���A
			//m_renderers.clear();

			// �������݊����҂�
			rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
			shadowMapNo++;
		}

		//�u���[�����s����
		/*for (auto& blur : m_blur)
		{
			blur.ExecuteOnGPU(rc, 1.0f);
		}*/

		//�����_�����O�G���W���̕��ł��
		UpDateLightCamera();
	}

	/*void Shadow::ShadowSpriteRender(RenderContext& rc)
	{
		shadowSprite.Update(
			{ FRAME_BUFFER_W / -2.0f,FRAME_BUFFER_H / 2.0f,0.0f },
			g_quatIdentity,
			g_vec3One,
			{ 0.0f,1.0f }
		);
		shadowSprite.Draw(rc);
	}*/

	void Shadow::SetCascadeNearAreaRates(
		float nearArea, float middleArea, float farArea)
	{
		// �������G���A�͈̔͂��ߋ����G���A�͈̔͂�菬�����Ȃ��Ă����
			// �v�Z��s�s�����N����̂ŁA�␳�B
		middleArea = max(nearArea + 0.01f, middleArea);
		// ���������G���A�͈̔͂��������G���A�͈̔͂�菬�����Ȃ��Ă����
		// �v�Z��s�s�����N����̂ŁA�␳�B
		farArea = max(middleArea + 0.01f, farArea);
		m_cascadeAreaRateArray[SHADOW_MAP_AREA_NEAR] = nearArea;
		m_cascadeAreaRateArray[SHADOW_MAP_AREA_MIDDLE] = middleArea;
		m_cascadeAreaRateArray[SHADOW_MAP_AREA_FAR] = farArea;
	}

	void Shadow::InitRenderTarget()
	{
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		DXGI_FORMAT colorFormat;
		DXGI_FORMAT depthFormat;

		colorFormat = g_hardShadowMapFormat.colorBufferFormat;
		depthFormat = g_hardShadowMapFormat.depthBufferFormat;

		//�߉e�p�̃V���h�E�}�b�v
		m_shadowMaps[0].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		//���e�p�̃V���h�E�}�b�v
		m_shadowMaps[1].Create(
			1024,
			1024,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);
		//���e�p�̃V���h�E�}�b�v
		m_shadowMaps[2].Create(
			512,
			512,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);

		//�V���h�E�}�b�v���ڂ������߂̃I�u�W�F�N�g�̏�����
		m_blur[0].Init(&m_shadowMaps[0].GetRenderTargetTexture());
		m_blur[1].Init(&m_shadowMaps[1].GetRenderTargetTexture());
		m_blur[2].Init(&m_shadowMaps[2].GetRenderTargetTexture());

	}

	void Shadow::InitLightCamera()
	{
		//�e�`�ʗp�̃��C�g�J�����̏�����
		//���C�g�J�����̈ʒu�B
		m_lightCamera.SetPosition(m_lightCameraPosition);
		//�J�����̒����_��ݒ�(�e�����Ƃ����ꏊ)
		m_lightCamera.SetTarget(0, 0, 0);
		//X��������ɂ���
		m_lightCamera.SetUp(1, 0, 0);
		//��p
		m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

		//�e�������Ȃ��悤�ɂ��邽�߂ɃJ�����𕽍s���e�ɂ���
		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);
		m_lightCamera.SetNear(1.0f);
		m_lightCamera.SetFar(10000.0f);

		//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���
		m_lightCamera.Update();
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

