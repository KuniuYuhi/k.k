#pragma once
#include "KnockBackInfo.h"

//���̃X�e�[�^�X���g���ăJ�[�u�f�[�^���쐬����B
//�J�[�u�f�[�^�������Ă���N���X�͂ǂ�����ł��A�N�Z�X�ł���悤�ɂ���
//

using namespace KnockBackInfo;

/// <summary>
/// �L�����N�^�[���m�b�N�o�b�N����Ƃ��ɃX�e�[�^�X�B
/// </summary>
class KnockBackStatus
{
public:

	/// <summary>
	/// �m�b�N�o�b�N�Ɋւ���X�e�[�^�X
	/// </summary>
	struct Status
	{
		int knockBackLevel = -1;				//�m�b�N�o�b�N���x���Blv0�`
		float initialVelocity = 0.0f;			//�����x
		float angle = 0;						//�p�x
		float duration = 0.0f;					//��������
		float gravity = 0.0f;					//�d��

		Vector2 knockBackTimeScale = g_vec2Zero;	//�m�b�N�o�b�N�̃X�s�[�h�BX�����ړ��AY���c�ړ�
	};

private:

	//�m�b�N�o�b�N�p�^�[���̐��̔z��
	Status m_status[enKnockBackPattern_Num];


	/// <summary>
	/// �m�b�N�o�b�N�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadKnockBackStatusCSV();

public:

	/// <summary>
	/// �m�b�N�o�b�N�p�^�[���̃X�e�[�^�X���擾
	/// </summary>
	/// <param name="getKnockBackPattern"></param>
	/// <returns></returns>
	Status& GetGetKnockBackStatus(EnKnockBackPattern getKnockBackPattern)
	{
		return m_status[getKnockBackPattern];
	}

	/// <summary>
	/// �m�b�N�o�b�N�̃X�e�[�^�X��������
	/// </summary>
	void InitKnockBackStatus();



	Vector2 GetKnockBackSpeed(EnKnockBackPattern getKnockBackPattern)
	{
		return m_status[getKnockBackPattern].knockBackTimeScale;
	}


	
	//CSV�̏���ۑ������C���X�^���X
	static KnockBackStatus StatusDataMAP;
	//�v���C���[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string knockBackStatusCsvFilePath;
	static bool IsLoadedKnockBackStatusCSVFile;


};

