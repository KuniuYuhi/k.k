#pragma once

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;

	class RenderingEngine
	{
	public:

		/// <summary>
		/// ���f�������_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.emplace_back(modelRender);
		}

		/// <summary>
		/// �X�v���C�g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="spriteRender"></param>
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.emplace_back(spriteRender);
		}

		/// <summary>
		/// �t�H���g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="fontRender"></param>
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.emplace_back(fontRender);
		}

		/// <summary>
		/// �`�揈�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ModelRendering(RenderContext& rc);

		/// <summary>
		/// �X�v���C�g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void SpriteRendering(RenderContext& rc);

		/// <summary>
		/// �t�H���g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void FontRendering(RenderContext& rc);

		std::vector<ModelRender*>			m_modelList;		//���f�����X�g
		std::vector<SpriteRender*>			m_spriteList;		//�X�v���C�g���X�g
		std::vector<FontRender*>			m_fontList;		//�X�v���C�g���X�g
	};
}


