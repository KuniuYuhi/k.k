#pragma once
#include "level2D/MapChip2D.h"

namespace nsK2EngineLow {

	struct CaslData;
	/// <summary>
	/// 2D�̃f�[�^
	/// </summary>
	struct Level2DObjectData
	{
		Vector3 position = Vector3::Zero;				//���W
		Quaternion rotation = Quaternion::Identity;		//��]
		Vector3 scale = Vector3::One;					//�g�嗦
		int width = 0;									//����
		int height = 0;									//�c��
		Vector2 pivot = Sprite::DEFAULT_PIVOT;			//�s�{�b�g
		const char* name;								//���O
		const char* ddsFilePath;						//dds�t�@�C���̃t�@�C���p�X

		/// <summary>
		/// �����œn�����I�u�W�F�N�g���������I�u�W�F�N�g�����ׂ�
		/// </summary>
		/// <param name="objName">���ׂ�I�u�W�F�N�g��</param>
		/// <returns>�I�u�W�F�N�g���������ꍇtrue��Ԃ�</returns>
		bool EqualObjectName(const char* objName)
		{
			return strcmp(objName, name) == 0;
		}

		/// <summary>
		/// ���O���O����v�����ׂ�
		/// </summary>
		/// <param name="n">���ׂ閼�O</param>
		/// <returns>��v���Ă�����true��Ԃ�</returns>
		bool ForwardMatchName(const char* n) const
		{
			auto len = strlen(n);
			auto namelen = strlen(name);
			if (len > namelen) {
				//���O�������̂ŕs��v
				return false;
			}
			//������̔�r
			return strncmp(n, name, len) == 0;
		}

	};

	/// <summary>
	/// ���x��2D
	/// </summary>
	class Level2DRender
	{
	private:
		using MapChip2D = std::unique_ptr<MapChip2D>;
	public:

		/// <summary>
		/// ���x����������
		/// </summary>
		/// <param name="filePath">casl�t�@�C���̃t�@�C���p�X</param>
		/// <param name="hookFunc">�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g</param>
		void Init(
			const char* filePath,
			std::function<bool(Level2DObjectData& objData)> hookFunc
		);

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update() const;

		/// <summary>
		/// �}�b�v�`�b�v2D�̕`�揈��
		/// </summary>
		/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
		void Draw(RenderContext& renderContext) const;

	private:

		/// <summary>
		/// �}�b�v�`�b�v2D��ǉ�����
		/// </summary>
		/// <param name="caslData">Casl�f�[�^</param>
		void AddMapChip2D(CaslData* objData);

	private:
		std::vector<MapChip2D> m_mapChip2DPtrList;
	};
}



