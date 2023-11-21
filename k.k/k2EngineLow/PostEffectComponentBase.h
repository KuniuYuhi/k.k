#pragma once

namespace nsK2EngineLow {
	class PostEffectComponentBase : public Noncopyable
	{
	public:
		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		/// <param name="depthRenderTarget">ZPrepase�����_�����O�^�[�Q�b�g</param>
		void Init(RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget);

		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

		/// <summary>
		/// �|�X�g�G�t�F�N�g�𖳌��ɂ���B
		/// </summary>
		void Disable()
		{
			m_isEnable = false;
		}
		/// <summary>
		/// �|�X�g�G�t�F�N�g��L���ɂ���B
		/// </summary>
		void Enable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// �|�X�g�G�t�F�N�g���L��������B
		/// </summary>
		/// <returns></returns>
		bool IsEnable() const
		{
			return m_isEnable;
		}
	private:
		/// <summary>
		/// �h���N���X�̏���������
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		/// <param name="depthRenderTarget"></param>
		virtual void OnInit(
			RenderTarget& mainRenderTarget,
			RenderTarget& depthRenderTarget
		) = 0;

		virtual void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) = 0;

		virtual Texture& GetResultTexture()
		{
			static Texture nullTexture;
			return nullTexture;
		}
		/// <summary>
		/// �|�X�g�̌��ʂ����C�������_�����O�^�[�Q�b�g�ɃR�s�[����H
		/// </summary>
		/// <returns></returns>
		virtual bool IsCopyResultTextureToMainRenderTarget() const
		{
			return false;
		}

	protected:
		bool m_isEnable = true;
		Sprite m_copyMainRtSprite;	// ���C�������_�����O�^�[�Q�b�g�ɕ`�悷�邽�߂̃X�v���C�g�B


	};

}