#pragma once
#include "KnockBackInfo.h"
#include "SummonerInfo.h"

using namespace KnockBackInfo;
using namespace SummonerSkillType;

class SummonerSkillStatus
{
private:

	/// <summary>
	/// �Z�Ɋւ���X�e�[�^�X
	/// </summary>
	struct SkillStatus
	{
		int power = 0;									//�U����
		float knockBackTimeScale = 0.0f;				//�m�b�N�o�b�N�̐i�s���x
		EnKnockBackPattern m_knockBackPattern;			//�m�b�N�o�b�N�p�^�[��

	};

	SkillStatus m_skillStatus[enSkillType_Num];

public:


	/// <summary>
	/// �Z�Ɋւ���X�e�[�^�X���擾
	/// </summary>
	/// <param name="getKnockBackPattern"></param>
	/// <returns></returns>
	SkillStatus& GetGetSkillTypeStatus(EnSkillSType skillType)
	{
		return m_skillStatus[skillType];
	}
	
	/// <summary>
	/// �m�b�N�o�b�N�̃X�e�[�^�X��������
	/// </summary>
	void InitSkillStatus();

	/// <summary>
	/// �Z�̍U���͂��擾
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	int& GetSkillPower(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].power;
	}
	/// <summary>
	/// �m�b�N�o�b�N�̐i�s���x���擾
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	float& GetSkillKnockBackTimeScale(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].knockBackTimeScale;
	}
	/// <summary>
	/// �m�b�N�o�b�N�̃p�^�[�����擾
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	EnKnockBackPattern& GetSkillKnockBackPattern(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].m_knockBackPattern;
	}


	//CSV�̏���ۑ������C���X�^���X
	static SummonerSkillStatus StatusDataMAP;
	//�v���C���[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string skillTypeStatusCsvFilePath;
	static bool IsLoadedSkillTypeStatusCSVFile;


private:
	/// <summary>
	/// �Z�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadSkillStatusCSV();


	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);

};


//�v�l�p�p�����[�^
//class 

