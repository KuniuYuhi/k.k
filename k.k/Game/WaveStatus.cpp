#include "stdafx.h"
#include "WaveStatus.h"
#include <fstream>
#include <sstream>


WaveStatus WaveStatus::StatusDataMAP;

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string WaveStatus::waveStatusCsvFilePath = "Assets/CSV/WaveStatus.csv";
bool WaveStatus::IsLoadedWaveStatusCSVFile = false;


//�E�F�[�u�̃X�e�[�^�X
// [�g�[�^���E�F�[�u��][�x�e�E�F�[�u�̔ԍ�]
// [1�E�F�[�u����][�E�F�[�u�Ԋu����][���萶����]
// [�ǉ�������][�����͈�(���a)][�����ł��Ȃ��͈�(�{�X)][�����ł��Ȃ��͈�(�v���C���[)]
// [�w���琶���܂ł̎���]

void WaveStatus::LoadWaveStatusCSV()
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(waveStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		WaveStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);

		//�g�[�^���E�F�[�u����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.totalWaves = stoi(buf);

		//�x�e�E�F�[�u�̔ԍ���ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.breakTimeWaveNumber = stoi(buf);

		//1�E�F�[�u���Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.waveTimeLimit = stof(buf);

		//�E�F�[�u�Ԋu���Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.shortBreakTime = stof(buf);

		//�G�l�~�[�ő吔��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.maxMobEnemys = stoi(buf);

		//���萶������ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.firstCreateMobEnemyAmount = stoi(buf);

		//�ǉ���������ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.addCreateMobEnemyAmonut = stoi(buf);

		//�����͈�(���a)��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.createRadius = stof(buf);

		//�������W���m�Œ���󂯂鋗����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.coordinateDistance = stof(buf);

		//�����ł��Ȃ��͈�(�{�X)��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.bossAvoidanceRadius = stof(buf);

		//�����ł��Ȃ��͈�(�v���C���[)��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.playerAvoidanceRadius = stof(buf);

		//�w���琶���܂ł̎��Ԃ�ǂݍ���
		getline(i_stream, buf, ',');
		statusData.m_status.incantationTimeLimit = stof(buf);

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP = statusData;

	}
	//���[�h����
	IsLoadedWaveStatusCSVFile = true;
	return;
}

void WaveStatus::InitWaveStatus()
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedWaveStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadWaveStatusCSV();
	}
	
	//�����Ɠ������O�̃f�[�^����
	*this = StatusDataMAP;

	return;

}
