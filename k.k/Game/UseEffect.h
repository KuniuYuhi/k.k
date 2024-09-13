#pragma once

#include "EffectNumbers.h"




using namespace EffectNumbers;

class CharacterBase;

//�G�t�F�N�g�̃v�[��
//���̃N���X�̃v�[�������




//���̃N���X
// ���̏�ł��邾��
//���W�𒆐S�ɒǂ�������
//�{�[���̈ʒu���擾���Ēǂ�������
//�����I�ɍ폜���鏈��

/// <summary>
/// �G�t�F�N�g���g���I�u�W�F�N�g���ǂݍ���
/// </summary>
class UseEffect:public IGameObject
{
public:



public:

	UseEffect();
	~UseEffect();


	bool Start() override;

	void Update() override;

	/// <summary>
	/// �G�t�F�N�g���v���C
	/// </summary>
	/// <param name="initEffectNumber">����������G�t�F�N�g�̔ԍ�</param>
	/// <param name="position">�������W</param>
	/// <param name="SetScale">�g�嗦</param>
	/// <param name="rotation">��]</param>
	/// <param name="isAutoMove">�����ňړ����邩</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// �G�t�F�N�g���v���C
	/// </summary>
	/// <param name="initEffectNumber">����������G�t�F�N�g�̔ԍ�</param>
	/// <param name="characterBase">�L�����N�^�[�x�[�X�C���X�^���X</param>
	/// <param name="position">�������W</param>
	/// <param name="SetScale">�g�嗦</param>
	/// <param name="rotation">��]</param>
	/// <param name="isAutoMove">�����ňړ����邩</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		CharacterBase* characterBase,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// �G�t�F�N�g���v���C
	/// </summary>
	/// <param name="initEffectNumber">����������G�t�F�N�g�̔ԍ�</param>
	/// <param name="characterBase">�L�����N�^�[�x�[�X�C���X�^���X</param>
	/// <param name="position">�������W</param>
	/// <param name="SetScale">�g�嗦</param>
	/// <param name="rotation">��]</param>
	/// <param name="isAutoMove">�����ňړ����邩</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		CharacterBase* characterBase,
		int boonId,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// �폜
	/// </summary>
	void Delete();

	/// <summary>
	/// �ړ����W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetMovePosition(Vector3 position);


private:

	/// <summary>
	/// ���f���̒��S��ǂ�������ړ�
	/// </summary>
	void ChaseMoveForCenterPosition();


	/// <summary>
	/// �w�肳�ꂽ���W���g���Ēǂ�������ړ�
	/// </summary>
	void ChaseMoveForSpecifiedPosition();




private:

	EffectEmitter* m_effect = nullptr;		//�G�t�F�N�g

	CharacterBase* m_character = nullptr;


	int m_boonId = -1;

	bool m_isAutoMove = false;		//�����ňړ������邩�H


};

