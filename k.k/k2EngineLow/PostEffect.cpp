#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
	void PostEffect::Init(RenderTarget& mainRenderTarget)
	{
		InitBloom(mainRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		m_bloom.OnRender(rc,mainRenderTarget);

		EffectEngine::GetInstance()->Draw();
	}

	void PostEffect::InitBloom(RenderTarget& mainRenderTarget)
	{
		m_bloom.Init(mainRenderTarget);
	}

}