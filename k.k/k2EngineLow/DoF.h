#pragma once

#include "PostEffectComponentBase.h"

/// <summary>
/// ��ʊE�[�x
/// </summary>
namespace nsK2EngineLow {
	class DoF : public PostEffectComponentBase
	{
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void OnInit(
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget) override;
		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) override;


		void Render(RenderContext& rc, RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);

		/// <summary>
		/// �|�X�g�G�t�F�N�g�����s�������ʂƂȂ�e�N�X�`�����擾�B
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture() override
		{
			return m_rtPhomboidBlur.GetRenderTargetTexture();
		}

	private:
		/// <summary>
		/// �����_�[�^�[�Q�b�g�̏���������
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// �ڂ����e�N�X�`���̏���������
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void InitBlurSprite(RenderTarget& mainRenderTarget);



	private:
		RenderTarget m_rtVerticalBlur;		//�����u���[�������邽�߂̃����_�����O�^�[�Q�b�g
		RenderTarget m_rtDiagonalBlur;		//�Ίp���u���[�������邽�߂̃����_�����O�^�[�Q�b�g
		RenderTarget m_rtPhomboidBlur;		//�Z�p�`�u���[�������邽�߂̃����_�����O�^�[�Q�b�g
		RenderTarget m_depthRenderTarget;
		Sprite m_combineBokeImageSprite;	//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɍ������邽�߂̃X�v���C�g
		Sprite m_vertDIagonalBlurSprite;	//�����A�Ίp���u���[�p�̃X�v���C�g
		Sprite m_phomboidBlurSprite;		//�Z�p�`�u���p�̃X�v���C�g
		GaussianBlur m_blur;
	};
}

