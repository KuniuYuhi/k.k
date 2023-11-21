#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
	void PostEffect::Init(
		RenderTarget& mainRenderTarget,
		RenderTarget& zprepassRenderTarget
	)
	{
		//ブルームの初期化処理
		m_bloom.Init(mainRenderTarget);
		//被写界深度の初期化処理
		m_depthOfFeild.Init(mainRenderTarget, zprepassRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		BeginGPUEvent("Bloom");
		//ブルーム
		OnRenderBloom(rc, mainRenderTarget);
		EndGPUEvent();

		g_renderingEngine->SetMainRenderTargetAndDepthStencilBuffer(rc);

		BeginGPUEvent("Dof");
		//被写界深度
		OnRenderDof(rc, mainRenderTarget);
		EndGPUEvent();
		

		//メインレンダーターゲットをPRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		EffectEngine::GetInstance()->Draw();
		//メインレンダーターゲットをTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	}

	void PostEffect::OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//ブルーム
		m_bloom.OnRender(rc, mainRenderTarget);
	}

	void PostEffect::OnRenderDof(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//被写界深度
		m_depthOfFeild.OnRender(rc, mainRenderTarget);
	}

	

}