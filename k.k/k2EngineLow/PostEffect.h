#pragma once
#include "Bloom.h"
#include "DoF.h"

namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		/// <summary>
		/// �|�X�g�G�t�F�N�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		void Init(
			RenderTarget& mainRenderTarget,
			RenderTarget& zprepassRenderTarget
		);

		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

		/// <summary>
		/// �u���[��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void OnRenderBloom(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// ��ʊE�[�x
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void OnRenderDof(RenderContext& rc, RenderTarget& mainRenderTarget);

		/// <summary>
		/// ��ʊE�[�x�̗L����
		/// </summary>
		void DofEnable()
		{
			m_depthOfFeild.Enable();
		}
		/// <summary>
		/// ��ʊE�[�x�̖�����
		/// </summary>
		void DofDisable()
		{
			m_depthOfFeild.Disable();
		}


	private:
		Bloom m_bloom;		//�u���[��
		DoF m_depthOfFeild;			//��ʊE�[�x
	};
}
