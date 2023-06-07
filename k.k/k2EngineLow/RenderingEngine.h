#pragma once

#include "Light.h"

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		/// <summary>
		/// �����_�[�^�[�Q�b�g�⃉�C�g�̏�����
		/// </summary>
		void Init();

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

		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns>�V�[�����C�g</returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/////////////////////////////////////////////////////
		///�f�B���N�V�������C�g�̊֐�
		/////////////////////////////////////////////////////
		
		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDerectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

		/////////////////////////////////////////////////////////////
		///�J�����̈ʒu�̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos">�J�����̍��W</param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

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

		std::vector<ModelRender*>		m_modelList;	//���f�����X�g
		std::vector<SpriteRender*>		m_spriteList;	//�X�v���C�g���X�g
		std::vector<FontRender*>		m_fontList;		//�X�v���C�g���X�g

		SceneLight						m_sceneLight;	//�V�[�����C�g
	};
}


