#include "stdafx.h"
#include "CommonEnemyStatus.h"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, CommonEnemyStatus> CommonEnemyStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string CommonEnemyStatus::commonEnemyStatusCsvFilePath = "Assets/CSV/CommonEnemyStatus.csv";

bool CommonEnemyStatus::IsLoadedCommonEnemyStatusCSVFile = false;


//�G�l�~�[�̃X�e�[�^�X
//[�̗�][�U����][��b�X�s�[�h][�U���C���^�[�o��]

void CommonEnemyStatus::InitCommonEnemyStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedCommonEnemyStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadCommonEnemyStatusCSV(name);
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

void CommonEnemyStatus::LoadCommonEnemyStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(commonEnemyStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		CommonEnemyStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//�̗͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.maxHp = stoi(buf);
		//�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.defaultPower = stoi(buf);
		//�X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.defaultSpeed = stof(buf);
		//�U���C���^�[�o����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.attackIntarval = stof(buf);
		//�ڋ߂ł��鋗����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.approachDistance = stof(buf);
		//�ҋ@���鋗����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.waitingDistance = stof(buf);
		
		//���݂�HP��ݒ�
		statusData.SetCurrentHp(statusData.m_commonEnemyStatus.maxHp);
		//���݂̍U���͂�ݒ�
		statusData.SetCurrentAtk(statusData.m_commonEnemyStatus.defaultPower);
	

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedCommonEnemyStatusCSVFile = true;
	return;
}
