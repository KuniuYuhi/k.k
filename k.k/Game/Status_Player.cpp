#include "stdafx.h"
#include "Status_Player.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, Status_Player> Status_Player::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string Status_Player::playerStatusCsvFilePath = "Assets/CSV/PlayerStatus.csv";
const std::string Status_Player::weaponStatusCsvFilePath = "Assets/CSV/WeaponStatus.csv";

bool Status_Player::IsLoadedPlayerStatusCSVFile = false;
bool Status_Player::IsLoadedWeaponStatusCSVFile = false;



//�v���C���[�̃X�e�[�^�X
//[���O][�̗�][�U����][�X�^�~�i][��b�X�s�[�h][�����X�s�[�h][����X�s�[�h]


//����̃X�e�[�^�X
//[][][][][][][][][]

void Status_Player::InitPlayerStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedPlayerStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadPlayerStatusCSV(name);
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

void Status_Player::InitWeaponStatus(const std::string& weaponName)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedWeaponStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadPlayerStatusCSV(name);
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

void Status_Player::LoadPlayerStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(playerStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		Status_Player statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//�̗͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.maxHp = stoi(buf);
		//�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.defaultPower = stoi(buf);
		//�X�^�~�i��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.maxStamina = stof(buf);
		//�X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.defaultSpeed = stof(buf);
		//�����X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.dashSpeed = stof(buf);
		//����X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.evasionSpeed = stof(buf);


		//���݂�HP��ݒ�
		statusData.m_playerStatus.currentHp = statusData.m_playerStatus.maxHp;
		//���݂̍U���͂�ݒ�
		statusData.m_playerStatus.currentPower = statusData.m_playerStatus.defaultPower;
		//���݂̃X�^�~�i��ݒ�
		statusData.m_playerStatus.currentStamina = statusData.m_playerStatus.maxStamina;

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedPlayerStatusCSVFile = true;
	return;
}

void Status_Player::LoadWeaponStatusCSV(const std::string& name)
{
}
