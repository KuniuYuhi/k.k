#pragma once

#include "PostEffectComponentBase.h"

/// <summary>
/// 被写界深度
/// </summary>
namespace nsK2EngineLow {
	class DoF : public PostEffectComponentBase
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void OnInit(
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget) override;
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) override;


		void Render(RenderContext& rc, RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);

		/// <summary>
		/// ポストエフェクトを実行した結果となるテクスチャを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture() override
		{
			return m_rtPhomboidBlur.GetRenderTargetTexture();
		}

	private:
		/// <summary>
		/// レンダーターゲットの初期化処理
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ぼかしテクスチャの初期化処理
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitBlurSprite(RenderTarget& mainRenderTarget);



	private:
		RenderTarget m_rtVerticalBlur;		//垂直ブラーをかけるためのレンダリングターゲット
		RenderTarget m_rtDiagonalBlur;		//対角線ブラーをかけるためのレンダリングターゲット
		RenderTarget m_rtPhomboidBlur;		//六角形ブラーをかけるためのレンダリングターゲット
		RenderTarget m_depthRenderTarget;
		Sprite m_combineBokeImageSprite;	//ボケ画像をメインレンダリングターゲットに合成するためのスプライト
		Sprite m_vertDIagonalBlurSprite;	//垂直、対角線ブラー用のスプライト
		Sprite m_phomboidBlurSprite;		//六角形ブラ用のスプライト
		GaussianBlur m_blur;
	};
}

