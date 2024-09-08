#include "stdafx.h"
#include "SummonerSkillStatus.h"
#include <fstream>
#include <sstream>

SummonerSkillStatus SummonerSkillStatus::StatusDataMAP;

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string SummonerSkillStatus::skillTypeStatusCsvFilePath = 
"Assets/CSV/EnemyUniqueStatus/BossEnemy/SummonerSkillStatus.csv";

bool SummonerSkillStatus::IsLoadedSkillTypeStatusCSVFile = false;


void SummonerSkillStatus::InitSkillStatus()
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedSkillTypeStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadSkillStatusCSV();
	}

	//�����Ɠ������O�̃f�[�^����
	*this = StatusDataMAP;

	return;
}

void SummonerSkillStatus::LoadSkillStatusCSV()
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(skillTypeStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);


	//�C���X�^���X
	SummonerSkillStatus statusData;

	//�z��̔ԍ�
	int num = 0;

	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);

		//�p�^�[���̖��O�͔�΂�
		getline(i_stream, buf, ',');

		//�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].power = stoi(buf);

		//�m�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].knockBackTimeScale = stof(buf);

		//�m�b�N�o�b�N�̃p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].m_knockBackPattern = StringToKnockBackPattern(buf);
		
		//���̔z��ɂ���
		num++;

	}


	//�}�b�v�Ɏ擾�����f�[�^����
	StatusDataMAP = statusData;

	//���[�h����
	IsLoadedSkillTypeStatusCSVFile = true;
	return;
}

EnKnockBackPattern SummonerSkillStatus::StringToKnockBackPattern(std::string& knockBackPattern)
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
