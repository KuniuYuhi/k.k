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

		/// <summary>
		/// �|�X�g�G�t�F�N�g�S�̂̕`�揈��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
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

		/// <summary>
		/// �|�X�g�G�t�F�N�g�̕`�揈��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="mainRenderTarget">���C�������_�����O�^�[�Q�b�g</param>
		virtual void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget) = 0;

		/// <summary>
		/// �e�N�X�`���̎擾
		/// </summary>
		/// <returns></returns>
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