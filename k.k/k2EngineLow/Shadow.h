#pragma once

namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		void Render(RenderContext& rc);

		void ShadowSpriteRender(RenderContext& rc);

		Texture& GetShadowMapTextrue()
		{
			return m_shadowMap.GetRenderTargetTexture();
		}

		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

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

		/// <summary>
		/// ���C�g�̍��W�X�V
		/// </summary>
		void UpDateLightCamera();

		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}

	private:
		RenderTarget m_shadowMap;
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(0.0f, 600.0f, 0.0f);

		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
