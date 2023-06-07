/*!
 *@brief	���x���B
 */

#pragma once
#include "level3D/TklFile.h"

namespace nsK2EngineLow {

	//wchar_t=���C�h�����B�������o�C�g�ɕϊ�����B
	//		  ������̏ꍇ�͍Ō�Ƀk�������������B
	//�@�@�@�@�����A������̎�O��L��t����B

	/// <summary>
	/// ���x���I�u�W�F�N�g�B
	/// </summary>
	struct LevelObjectData {
		Vector3			position;			//���W
		Quaternion		rotation;			//��]
		Vector3			scale;			//�g�嗦
		const			wchar_t* name;		//���O
		int				number = 0;			//�ԍ�

		/// <summary>
		/// �����œn�����I�u�W�F�N�g���������I�u�W�F�N�g�����ׂ�
		/// </summary>
		/// <param name="objName">���ׂ�I�u�W�F�N�g��</param>
		/// <returns>�I�u�W�F�N�g���������ꍇtrue��Ԃ�</returns>
		bool EqualObjectName(const wchar_t* objName)
		{
			//���C�h������̔�r
			return wcscmp(objName, name) == 0;
		}

		/// <summary>
		/// ���O���O����v�����ׂ�
		/// </summary>
		/// <param name="n">���ׂ閼�O</param>
		/// <returns>��v���Ă�����true��Ԃ�</returns>
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//���O�������̂ŕs��v
				return false;
			}
			//���C�h������̔�r
			return wcsncmp(n, name, len) == 0;
		}
	};


	class Level3DRender
	{
	public:
		~Level3DRender();
		/*!
		 * @brief	���x�����������B
		 *@param[in]	levelDataFilePath		tkl�t�@�C���̃t�@�C���p�X�B
		 *@param[in] hookFunc				�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g�B
		 *   �t�b�N���Ȃ��Ȃ�nullptr���w�肷��΂悢�A
		 * ���̊֐��I�u�W�F�N�g��false��Ԃ��ƁA�I�u�W�F�N�g�̏�񂩂�A
		 * �ÓI�I�u�W�F�N�g��MapChip�N���X�̃C���X�^���X����������܂��B
		 * �I�u�W�F�N�g�̖��O�ȂǂŁA�h�A��W�����v��A�A�C�e���Ȃǂ̓���ȃN���X�̃C���X�^���X�𐶐������Ƃ��ɁA
		 * �f�t�H���g�ō쐬�����MapChip�N���X�̃C���X�^���X���s�v�ȏꍇ��true��Ԃ��Ă��������B
		 * �Ⴆ�΁A�t�b�N�֐��̒��ŁA�n���ꂽ�I�u�W�F�N�g�f�[�^�̖��O�̃��f����`�悷��N���X�̃C���X�^���X��
		 * ���������Ƃ��ɁAfalse��Ԃ��Ă��܂��ƁA�������f������`�悳��邱�ƂɂȂ�܂��B
		 */


		/// <summary>
		/// ���x����������
		/// </summary>
		/// <param name="filePath">tkl�t�@�C���̃t�@�C���p�X</param>
		/// <param name="hookFunk">�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g</param>
		void Init(
			const char* filePath,
			std::function<bool(LevelObjectData& objData)> hookFunc
		);

	private:
		//tkl�t�@�C���̍s���ϊ�����
		void MatrixTklToLevel();
	private:
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bonelist;
		TklFile m_tklFile;
	};
}


