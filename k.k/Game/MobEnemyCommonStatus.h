#pragma once

#include "KnockBackInfo.h"

using namespace KnockBackInfo;

/// <summary>
/// ���u�G�l�~�[�̊�b�X�e�[�^�X
/// </summary>
class MobEnemyCommonStatus
{
private:

	struct MobStatus
	{
		//�ʏ�U���̃m�b�N�o�b�N�p�^�[��
		EnKnockBackPattern normalAttackKnockBackPattern;

		float normalAttackTimeScale = 1.0f;		//�ʏ�U���̃m�b�N�o�b�N�̐i�s���x 

	};


	MobStatus m_mobStatus;

public:

	const MobStatus& GetMobCommonStatus()
	{
		return m_mobStatus;
	}

	/// <summary>
	/// ���u�G�l�~�[�̊�{�X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X�����������������u�G�l�~�[�̖��O</param>
	void InitMobEnemyCommonStatus(const std::string& name);




	EnKnockBackPattern GetNormalAttackKnockBackPattern()
	{
		return m_mobStatus.normalAttackKnockBackPattern;
	}
	

	const float& GetNormalAttackTimeScale()
	{
		return m_mobStatus.normalAttackTimeScale;
	}



	//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, MobEnemyCommonStatus> StatusDataMAP;
	//�G�l�~�[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string mobEnemyCommonStatusCsvFilePath;
	static bool IsLoadedmobEnemyCommonStatusCSVFile;


public:
	/// <summary>
	/// ���u�G�l�~�[�̊�{�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂������u�G�l�~�[�̖��O</param>
	void LoadEnemyCommonStatusCSV(const std::string& name);



	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);

};

