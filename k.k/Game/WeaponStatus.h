#pragma once

#include "WeaponInfo.h"
#include "KnockBackInfo.h"

using namespace WeaponAttribute;
using namespace KnockBackInfo;

//�U���̎d�g��
//
//�_���[�W��^�����畐��X�e�[�^�X�̃R���{���Ƃ̃m�b�N�o�b�N�p�^�[�����g����
// �J�[�u�f�[�^�N���X����vector�^�̃f�[�^���擾

/// <summary>
/// ����̃X�e�[�^�X
/// </summary>
class WeaponStatus
{
private:

	enum EnCombo
	{
		FirstCombo = 1,
		SecondCombo,
		ThirdCombo
	};

	/// <summary>
	/// �X�e�[�^�X
	/// </summary>
	struct CommonStatus
	{
		int defaultPower = 0;				//��b�U����
		int currentPower = 0;				//���݂̍U����
		int skillPower = 0;					//�X�L���U����
		float skillStaminaCost = 0;			//�X�L���̃X�^�~�i�����
		float defensiveStaminaCost = 0;		//����A�h��̃X�^�~�i�����

		EnWeaponAttribute weaponAttribute;	//����̑���

		//�ʏ�U���̃m�b�N�o�b�N�p�^�[��
		EnKnockBackPattern firstComboPattern;		//1�R���{
		EnKnockBackPattern secondComboPattern;	//2�R���{
		EnKnockBackPattern thirdComboPattern;		//3�R���{

		//�X�L���̃m�b�N�o�b�N�p�^�[��
		EnKnockBackPattern m_SkillPattern;			//�X�L��

	};

	CommonStatus m_status;

public:


	const CommonStatus GetCommonStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ����̃X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X������������������̖��O</param>
	void InitWeaponCommonStatus(const std::string& name);





	/// <summary>
	/// ���݂̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetCurrentPower(const int setCurrentPower)
	{
		m_status.currentPower = setCurrentPower;
	}
	/// <summary>
	/// ���݂̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentPower() const
	{
		return m_status.currentPower;
	}

	/// <summary>
	/// �X�L���U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetSkillPower(const int setSkillPower)
	{
		m_status.skillPower = setSkillPower;
	}
	/// <summary>
	/// �X�L���U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetSkillPower() const
	{
		return m_status.skillPower;
	}

	/// <summary>
	/// ����̑������擾
	/// </summary>
	/// <returns></returns>
	const EnWeaponAttribute& GetWeaponAttribute()
	{
		return m_status.weaponAttribute;
	}

	/// <summary>
	/// �X�L���̃X�^�~�i����ʂ��擾
	/// </summary>
	/// <returns></returns>
	const float& GetSkillStaminaCost()
	{
		return m_status.skillStaminaCost;
	}

	/// <summary>
	/// ����A�h��̃X�^�~�i����ʂ��擾
	/// </summary>
	/// <returns></returns>
	const float& GetDefensiveStaminaCost()
	{
		return m_status.defensiveStaminaCost;
	}

	const EnKnockBackPattern& GetComboKnockBackPattern(EnCombo comboCount)
	{
		switch (comboCount)
		{
		case WeaponStatus::FirstCombo:
			return m_status.firstComboPattern;
		case WeaponStatus::SecondCombo:
			return m_status.secondComboPattern;;
		case WeaponStatus::ThirdCombo:
			return m_status.thirdComboPattern;;
		default:
			std::abort();
			break;
		}
	}

	//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, WeaponStatus> StatusDataMAP;
	//�v���C���[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string weaponCommonStatusCsvFilePath;
	static bool IsLoadedWeaponCommonStatusCSVFile;



private:

	/// <summary>
	/// ����X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂�������̖��O</param>
	void LoadWeaponCommonStatusCSV(const std::string& name);



	EnWeaponAttribute StringToWeaponAttribute(std::string& attributeName);

	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);



};

