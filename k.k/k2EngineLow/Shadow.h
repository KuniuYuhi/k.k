#pragma once

namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();
		//void Render(RenderContext& rc);

		void Render(
			RenderContext& rc,
			int ligNo,
			Vector3& lightDirection,
			std::vector< IRenderer* >& renderObjects
		);

		void ShadowSpriteRender(RenderContext& rc);

		Texture& GetShadowMapTextrue()
		{
			return m_shadowMaps[0].GetRenderTargetTexture();

			//return m_shadowMap.GetRenderTargetTexture();
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
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];          // シャドウマップ
		Camera m_lightCamera;

		Vector3 m_lightCameraPosition = Vector3(0.0f, 1200.0f, 0.0f);	//カメラの座標(座標からターゲットに向かって影が落とされる)
		std::vector< IRenderer* > m_renderers;              // シャドウマップへのレンダラーの配列。
		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
