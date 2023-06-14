#pragma once
#include "Bloom.h"

namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		/// <summary>
		/// �|�X�g�G�t�F�N�g�̏�����
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
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
