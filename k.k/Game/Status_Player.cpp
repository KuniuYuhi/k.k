#include "stdafx.h"
#include "Status_Player.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, Status_Player> Status_Player::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string Status_Player::playerStatusCsvFilePath = "Assets/CSV/PlayerStatus.csv";

bool Status_Player::IsLoadedPlayerStatusCSVFile = false;



//�v���C���[�̃X�e�[�^�X
//[���O][�̗�][�U����][�X�^�~�i][�X�^�~�i�̉񕜑��x�{��][��b�X�s�[�h][�����X�s�[�h][����X�s�[�h]


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
		//�X�^�~�i�̉񕜑��x�{����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.staminaRecoveryRate = stof(buf);
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


bool Status_Player::TryConsumeStamina(float consumeValue)
{
	//���̃X�^�~�i���`
	float tempStamina = m_playerStatus.currentStamina;

	//�X�^�~�i�������
	tempStamina -= consumeValue;

	//�X�^�~�i��0�ȉ��ɂȂ�����
	if (tempStamina < 0)
	{
		//�X�^�~�i������ł��Ȃ��B�s���ł��Ȃ�
		return false;
	}

	//����ꂽ��̃X�^�~�i���X�e�[�^�X�̃X�^�~�i�ɑ��
	m_playerStatus.currentStamina = tempStamina;

	return true;
}


bool Status_Player::CheckConsumeStamina(float checkValue)
{
	//���̃X�^�~�i���`
	float tempStamina = m_playerStatus.currentStamina;

	//�X�^�~�i�������
	tempStamina -= checkValue;

	//�X�^�~�i��0�ȉ��ɂȂ�����
	if (tempStamina < 0)
	{
		//�X�^�~�i������ł��Ȃ��B�s���ł��Ȃ�
		return false;
	}
	//����ł����Ԃɂ���
	return true;
}

void Status_Player::RecoveryStamina(float recoveryValue)
{
	//���ɍő�l�܂ŉ񕜂��Ă���Ȃ珈�����Ȃ�
	if (m_playerStatus.currentStamina == m_playerStatus.maxStamina)
	{
		return;
	}

	m_playerStatus.currentStamina += recoveryValue;
	//�X�^�~�i���ő�l�𒴂��Ȃ��悤�ɂ���
	if (m_playerStatus.currentStamina >= m_playerStatus.maxStamina)
	{
		m_playerStatus.currentStamina = m_playerStatus.maxStamina;
	}

}
