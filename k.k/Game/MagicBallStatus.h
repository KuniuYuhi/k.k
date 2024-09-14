#pragma once

#include "KnockBackInfo.h"

using namespace KnockBackInfo;

/// <summary>
/// ���@���Ɋւ���X�e�[�^�X
/// </summary>
class MagicBallStatus
{
private:

	struct Status
	{
		int power = 0;					//�U����
		float speed = 0.0f;				//�X�s�[�h
		float deleteTimeLimit = 0.0f;		//���ł��鎞��

		EnKnockBackPattern knockBackPattern;		//�m�b�N�o�b�N�̃p�^�[��

		float knockBackTimeScale = 0.0f;			//�m�b�N�o�b�N�̐i�s���x

	};

	Status m_Status;

public:


	Status GetStatus()
	{
		return m_Status;
	}

	/// <summary>
	/// ���@���X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X�����������������@���̖��O</param>
	void InitMagicBallStatus(const std::string& name);


	const int& GetPower()
	{
		return m_Status.power;
	}

	const float& GetSpeed()
	{
		return m_Status.speed;
	}

	void SetDeleteTimeLimit(float timeLimit)
	{
		m_Status.deleteTimeLimit = timeLimit;
	}

	const float& GetDeleteTimeLimit()
	{
		return m_Status.deleteTimeLimit;
	}

	EnKnockBackPattern GetKnockBackPattern()
	{
		return m_Status.knockBackPattern;
	}

	const float& GetKnockBackTimeScale()
	{
		return m_Status.knockBackTimeScale;
	}

	
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, MagicBallStatus> StatusDataMAP;
	//���@���̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string magicBallStatusCsvFilePath;
	static bool IsLoadedMagicBallStatusCSVFile;

private:

	/// <summary>
	/// ���@���̃X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂������@���̖��O</param>
	void LoadMagicBallStatusCSV(const std::string& name);


	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);
};

