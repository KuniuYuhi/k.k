#include "stdafx.h"
#include "WeaponStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, WeaponStatus> WeaponStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string WeaponStatus::weaponCommonStatusCsvFilePath = "Assets/CSV/WeaponCommonStatus.csv";

bool WeaponStatus::IsLoadedWeaponCommonStatusCSVFile = false;



// [��b�U����][���݂̍U����][�X�L���U����]
// [�X�L���̃X�^�~�i�����][����A�h��̃X�^�~�i�����][����̑���]
// [1�R���{�ڃm�b�N�o�b�N�p�^�[��][2�R���{�ڃm�b�N�o�b�N�p�^�[��][3�R���{�ڃm�b�N�o�b�N�p�^�[��]
// [�X�L���̃m�b�N�o�b�N�p�^�[��]


void WeaponStatus::InitWeaponCommonStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedWeaponCommonStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadWeaponCommonStatusCSV(name);
	}
	//�L�[��name�ł���v�f��T��
	auto itr = StatusDataMAP.find(name);
	//���O���Ȃ�
	if (itr == StatusDataMAP.end())
	{
		std::abort();
		return;
	}
	//�����Ɠ������O�̃f�[�^����
	*this = itr->second;

	return;
}

void WeaponStatus::LoadWeaponCommonStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(weaponCommonStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		WeaponStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		
		//��b�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.defaultPower = stoi(buf);

		//�X�L���U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.skillPower = stoi(buf);

		//�X�L���U���̃X�^�~�i����ʂ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.skillStaminaCost = stof(buf);

		//����A�h��̃X�^�~�i����ʂ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.defensiveStaminaCost = stof(buf);

		//����̑�����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.weaponAttribute = StringToWeaponAttribute(buf);

		//1�R���{�ڂ̃m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.firstComboPattern = StringToKnockBackPattern(buf);
		
		//2�R���{�ڂ̃m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.secondComboPattern = StringToKnockBackPattern(buf);

		//3�R���{�ڂ̃m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.thirdComboPattern = StringToKnockBackPattern(buf);

		//�X�L���̃m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.m_SkillPattern = StringToKnockBackPattern(buf);


		//���݂̍U���͂�ݒ�
		statusData.SetCurrentPower(statusData.m_status.defaultPower);

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedWeaponCommonStatusCSVFile = true;
	return;
}

EnWeaponAttribute WeaponStatus::StringToWeaponAttribute(std::string& attributeName)
{
	//�a�^�C�v�Ȃ�
	if (attributeName == "Slash")
	{
		return EnWeaponAttribute::enWeaponAttribute_Slash;
	}
	//�˃^�C�v�Ȃ�
	else if(attributeName == "Pierce")
	{
		return EnWeaponAttribute::enWeaponAttribute_Pierce;
	}
	//�����܂ŗ����炢���Ȃ�
	std::abort();
	return EnWeaponAttribute::enWeaponAttribute_None;
}

EnKnockBackPattern WeaponStatus::StringToKnockBackPattern(std::string& knockBackPattern)
{
	//�n�ʂɐڂ����܂܏����ジ����p�^�[��
	if (knockBackPattern == "ShortGroundedRetreat")
	{
		return EnKnockBackPattern::enKBPattern_ShortGroundedRetreat;
	}
	//�n�ʂɐڂ����܂ܒ��߂Ɍジ����p�^�[��
	else if (knockBackPattern == "LongGroundedRetreat")
	{
		return EnKnockBackPattern::enKBPattern_LongGroundedRetreat;
	}
	//����������тȂ���ジ����p�^�[��
	else if (knockBackPattern == "SlightAirborneRetreat")
	{
		return EnKnockBackPattern::enKBPattern_SlightAirborneRetreat;
	}
	//���߂ɐ�����тȂ���ジ����p�^�[��
	else if (knockBackPattern == "FullAirborneRetreat")
	{
		return EnKnockBackPattern::enKBPattern_FullAirborneRetreat;
	}
	//���̏ꂩ�瓮�����ɋ󒆂ɐ������(�󒆂ɕ���)
	else if (knockBackPattern == "VerticalLaunch")
	{
		return EnKnockBackPattern::enKBPattern_VerticalLaunch;
	}

	//�����܂ŗ����炢���Ȃ�
	std::abort();
	return EnKnockBackPattern::enKnockBackPattern_Num;

}
