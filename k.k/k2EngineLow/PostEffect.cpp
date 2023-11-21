#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
	void PostEffect::Init(
		RenderTarget& mainRenderTarget,
		RenderTarget& zprepassRenderTarget
	)
	{
		//�u���[���̏���������
		m_bloom.Init(mainRenderTarget);
		//��ʊE�[�x�̏���������
		m_depthOfFeild.Init(mainRenderTarget, zprepassRenderTarget);
	}

	void PostEffect::Render(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		BeginGPUEvent("Bloom");
		//�u���[��
		OnRenderBloom(rc, mainRenderTarget);
		EndGPUEvent();

		g_renderingEngine->SetMainRenderTargetAndDepthStencilBuffer(rc);

		BeginGPUEvent("Dof");
		//��ʊE�[�x
		OnRenderDof(rc, mainRenderTarget);
		EndGPUEvent();
		

		//���C�������_�[�^�[�Q�b�g��PRESENT����RENDERTARGET�ցB
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		EffectEngine::GetInstance()->Draw();
		//���C�������_�[�^�[�Q�b�g��TARGET����PRESENT�ցB
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	}

	void PostEffect::OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//�u���[��
		m_bloom.OnRender(rc, mainRenderTarget);
	}

	void PostEffect::OnRenderDof(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//��ʊE�[�x
		m_depthOfFeild.OnRender(rc, mainRenderTarget);
	}

	

}