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

	void PostEffect::Render(
		RenderContext& rc, 
		RenderTarget& mainRenderTarget,
		RenderTarget& zprepassRenderTarget)
	{
		BeginGPUEvent("Bloom");
		//ブルームの描画処理
		OnRenderBloom(rc, mainRenderTarget);
		EndGPUEvent();

		g_renderingEngine->SetMainRenderTargetAndDepthStencilBuffer(rc);

		BeginGPUEvent("Dof");
		//被写界深度の描画処理
		OnRenderDof(rc, mainRenderTarget, zprepassRenderTarget);
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

	void PostEffect::OnRenderDof(
		RenderContext& rc, 
		RenderTarget& mainRenderTarget,
		RenderTarget& zprepassRenderTarget
	)
	{
		//被写界深度
		m_depthOfFeild.Render(
			rc,
			mainRenderTarget,
			zprepassRenderTarget
		);
	}

	

}