#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		/// <summary>
		/// ポストエフェクトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Init(RenderTarget& mainRenderTarget);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		void InitBloom(RenderTarget& mainRenderTarget);

	private:
		Bloom m_bloom;
	};
}
