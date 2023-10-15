#pragma once

namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		//void Render(RenderContext& rc);

		void Render(
			RenderContext& rc,
			Vector3& lightDirection,
			std::vector< IRenderer* >& renderObjects
		);

		void ShadowSpriteRender(RenderContext& rc);

		Texture& GetShadowMapTextrue()
		{
			return m_shadowMaps[0].GetRenderTargetTexture();

			//return m_shadowMap.GetRenderTargetTexture();
		}

		/// <summary>
		/// �V���h�E�}�b�v���擾
		/// </summary>
		/// <param name="areaNo">�G���A�ԍ�</param>
		/// <returns></returns>
		Texture& GetShadowMap(int areaNo)
		{
			/*if (m_isSoftShadow) {
				return m_blur[areaNo].GetBokeTexture();
			}*/
			return m_shadowMaps[areaNo].GetRenderTargetTexture();
		}

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

		/// <summary>
		/// 
		/// </summary>
		void InitShadowSprite();

		

		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}

	private:
		RenderTarget m_shadowMap;
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];          // �V���h�E�}�b�v
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(500.0f, 1200.0f, 0.0f);	//�J�����̍��W(���W����^�[�Q�b�g�Ɍ������ĉe�����Ƃ����)
		std::vector< IRenderer* > m_renderers;              // �V���h�E�}�b�v�ւ̃����_���[�̔z��B
		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
