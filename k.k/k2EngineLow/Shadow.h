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
		/// レンダリングターゲットの初期化
		/// </summary>
		void InitRenderTarget();

		/// <summary>
		/// 影描写用のカメラの初期化
		/// </summary>
		void InitLightCamera();

		/// <summary>
		/// 
		/// </summary>
		void InitShadowSprite();

		/// <summary>
		/// ライトの座標更新
		/// </summary>
		void UpDateLightCamera();

		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}

	private:
		RenderTarget m_shadowMap;
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(100.0f, 700.0f, 300.0f);	//カメラの座標(座標からターゲットに向かって影が落とされる)

		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
