#pragma once

#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();

		void Render(
			RenderContext& rc,
			Camera& lightCamera,
			std::vector< IRenderer* >& renderObjects
		);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rc"></param>
		//void ShadowSpriteRender(RenderContext& rc);

		/// <summary>
		/// �J�X�P�[�h�V���h�E�̃G���A����ݒ�B
		/// </summary>
		/// <remark>
		/// �Q�[���J�����̋ߕ��ʂ��牓���ʂ܂ł̃G���A�́A��%�͈̔͂��ߋ����p�̍��𑜓x�̃V���h�E�}�b�v�ɁA
		/// ��%�𒆋����p�̃V���h�E�}�b�v�ɁA��%���������p�̃V���h�E�}�b�v�ɕ`�悷�邩���w�肵�����́B
		/// �Ⴆ�΁AnearArea  = 0.1(10%)�AmiddleArea = 0.3(30%), farArea = 0.8(80%)�Ǝw�肳��Ă�����A
		/// �J��������10%�̃G���A�͋ߋ����p�̃V���h�E�}�b�v�ɕ`��A
		/// �J��������10%�`30%�̃G���A�͒������p�̃V���h�E�}�b�v�ɕ`��A
		/// �J��������30%�`80%�̃G���A�͉������p�̃V���h�E�}�b�v�ɕ`��ƂȂ�B
		/// �e�̕i���̒����Ɏg���Ă��������B
		/// </remark>
		/// <param name="nearArea">�߉e�G���A���B</param>
		void SetCascadeNearAreaRates(float nearArea, float middleArea, float farArea);

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s����擾
		/// </summary>
		/// <param name="areaNo"></param>
		/// <returns></returns>
		const Matrix& GetLVPMatrix(int areaNo) const
		{
			return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
		}

		/// <summary>
		/// �V���h�E�}�b�v���擾
		/// </summary>
		/// <param name="areaNo">�G���A�ԍ�</param>
		/// <returns></returns>
		Texture& GetShadowMap(int areaNo)
		{
			//return m_blur[areaNo].GetBokeTexture();
			/*if (m_isSoftShadow) {
				return m_blur[areaNo].GetBokeTexture();
			}*/
			return m_shadowMaps[areaNo].GetRenderTargetTexture();
		}


		/// <summary>
		/// ���C�g�J�����̎擾
		/// </summary>
		/// <returns></returns>
		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}
		/// <summary>
		/// ���C�g�̍��W�X�V
		/// </summary>
		void UpDateLightCamera();


	private:
		/// <summary>
		/// �����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTarget();

		/// <summary>
		/// �e�`�ʗp�̃J�����̏�����
		/// </summary>
		void InitLightCamera();

	private:
		CascadeShadowMapMatrix m_cascadeShadowMapMatrix;		//�J�X�P�[�h�V���h�E�̍s��������I�u�W�F�N�g
		//RenderTarget m_shadowMap;
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];		//�V���h�E�}�b�v
		GaussianBlur m_blur[NUM_SHADOW_MAP];			//�V���h�E�}�b�v�Ƀu���[�������鏈��
		Camera m_lightCamera;
		float m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 800.0f,2000.0f, g_camera3D->GetFar()};//�߉e�A���e�A���e�͈̔́B�������Ă���
		Vector3 m_lightCameraPosition = Vector3(500.0f, 1200.0f, 0.0f);	//�J�����̍��W(���W����^�[�Q�b�g�Ɍ������ĉe�����Ƃ����)
		std::vector< IRenderer* > m_renderers;              // �V���h�E�}�b�v�ւ̃����_���[�̔z��B
		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
