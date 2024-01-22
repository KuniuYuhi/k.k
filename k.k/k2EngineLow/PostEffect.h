#pragma once
#include "Bloom.h"
#include "DoF.h"

namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		/// <summary>
		/// ポストエフェクトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Init(
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget
		);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Render(
			RenderContext& rc, 
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget
		);

		/// <summary>
		/// ブルーム
		/// </summary>
		/// <param name="rc"><レンダーコンテキスト/param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// 被写界深度
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void OnRenderDof(
			RenderContext& rc, 
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget
		);

		/// <summary>
		/// 被写界深度の有効化
		/// </summary>
		void DofEnable()
		{
			m_depthOfFeild.Enable();
		}
		/// <summary>
		/// 被写界深度の無効化
		/// </summary>
		void DofDisable()
		{
			m_depthOfFeild.Disable();
		}


	private:
		Bloom m_bloom;		//ブルーム
		DoF m_depthOfFeild;			//被写界深度
	};
}
