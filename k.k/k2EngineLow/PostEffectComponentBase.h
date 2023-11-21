#pragma once

namespace nsK2EngineLow {
	class PostEffectComponentBase : public Noncopyable
	{
	public:
		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		/// <param name="depthRenderTarget">ZPrepaseレンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);

		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

		/// <summary>
		/// ポストエフェクトを無効にする。
		/// </summary>
		void Disable()
		{
			m_isEnable = false;
		}
		/// <summary>
		/// ポストエフェクトを有効にする。
		/// </summary>
		void Enable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// ポストエフェクトが有効か判定。
		/// </summary>
		/// <returns></returns>
		bool IsEnable() const
		{
			return m_isEnable;
		}
	private:
		/// <summary>
		/// 派生クラスの初期化処理
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		/// <param name="depthRenderTarget"></param>
		virtual void OnInit(
			RenderTarget& mainRenderTarget,
			RenderTarget& depthRenderTarget
		) = 0;

		virtual void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) = 0;

		virtual Texture& GetResultTexture()
		{
			static Texture nullTexture;
			return nullTexture;
		}
		/// <summary>
		/// ポストの結果をメインレンダリングターゲットにコピーする？
		/// </summary>
		/// <returns></returns>
		virtual bool IsCopyResultTextureToMainRenderTarget() const
		{
			return false;
		}

	protected:
		bool m_isEnable = true;
		Sprite m_copyMainRtSprite;	// メインレンダリングターゲットに描画するためのスプライト。


	};

}