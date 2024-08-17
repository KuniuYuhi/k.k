#include "stdafx.h"
#include "GreateSwordStatus.h"
#include <fstream>
#include <sstream>

GreateSwordStatus GreateSwordStatus::StatusDataMAP;

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string GreateSwordStatus::uniqueStatusCsvFilePath = "Assets/CSV/WeaponUniqueStatus/GreateSwordStatus.csv";
bool GreateSwordStatus::IsLoadedUniqueStatusCSVFile = false;


void GreateSwordStatus::LoadUniqueStatusCSV()
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(uniqueStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		GreateSwordStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);

		//��R���{�ڃm�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.firstAttackTimeScale = stof(buf);

		//��R���{�ڃm�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.secondAttackTimeScale = stof(buf);

		//�O�R���{�ڃm�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.thirdAttackTimeScale = stof(buf);

		//�X�L���U���̃m�b�N�o�b�N�̐i�s���x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.skillAttackTimeScale = stof(buf);

		//����X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.defenciveMoveSpeed = stof(buf);

		for (int i = 0; i < 3; i++)
		{
			//�ʏ�U���̃X�s�[�h��ǂݍ��ށB
			getline(i_stream, buf, ',');
			statusData.m_uniqueStatus.normalAttackSpeed[i] = stof(buf);
		}

		//�ʏ�U���̍��G�͈�(���a)��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.normalAttackSearchRadius = stof(buf);

		//�ʏ�U���Ōv�Z����x�N�g�����m�̓��ς̔�r�Ώۂ̓��ς�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.normalAttackComparisonDot = stof(buf);

		//�X�L���̑؋󐧌����Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.skillFlightTimeLimit = stof(buf);



		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP = statusData;

	}
	//���[�h����
	IsLoadedUniqueStatusCSVFile = true;
	return;
}

void GreateSwordStatus::InitUniqueStatus()
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedUniqueStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadUniqueStatusCSV();
	}

	//�����Ɠ������O�̃f�[�^����
	*this = StatusDataMAP;

	return;
}
