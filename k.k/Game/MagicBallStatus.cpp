#include "stdafx.h"
#include "MagicBallStatus.h"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, MagicBallStatus> MagicBallStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string MagicBallStatus::magicBallStatusCsvFilePath = "Assets/CSV/MagicBallStatus.csv";

bool MagicBallStatus::IsLoadedMagicBallStatusCSVFile = false;



void MagicBallStatus::InitMagicBallStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedMagicBallStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadMagicBallStatusCSV(name);
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

void MagicBallStatus::LoadMagicBallStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(magicBallStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		MagicBallStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_Status.power = stoi(buf);

		//�X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_Status.speed = stof(buf);
		
		//���Ŏ��Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_Status.deleteTimeLimit = stof(buf);

		//�m�b�N�o�b�N�p�^�[����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_Status.knockBackPattern = StringToKnockBackPattern(buf);

		//�m�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_Status.knockBackTimeScale = stof(buf);


		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedMagicBallStatusCSVFile = true;
	return;

}

EnKnockBackPattern MagicBallStatus::StringToKnockBackPattern(std::string& knockBackPattern)
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
