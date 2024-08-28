#include "stdafx.h"
#include "MobEnemyCommonStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, MobEnemyCommonStatus> MobEnemyCommonStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string MobEnemyCommonStatus::mobEnemyCommonStatusCsvFilePath = "Assets/CSV/EnemyUniqueStatus/MobEnemy/MobEnemyCommonStatus.csv";

bool MobEnemyCommonStatus::IsLoadedmobEnemyCommonStatusCSVFile = false;



void MobEnemyCommonStatus::InitMobEnemyCommonStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedmobEnemyCommonStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadEnemyCommonStatusCSV(name);
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

void MobEnemyCommonStatus::LoadEnemyCommonStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(mobEnemyCommonStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		MobEnemyCommonStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//�ʏ�U���̃m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_mobStatus.normalAttackKnockBackPattern = StringToKnockBackPattern(buf);

		//�ʏ�U���̃m�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_mobStatus.normalAttackTimeScale = stof(buf);


		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedmobEnemyCommonStatusCSVFile = true;
	return;

}

EnKnockBackPattern MobEnemyCommonStatus::StringToKnockBackPattern(std::string& knockBackPattern)
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
