#include "stdafx.h"
#include "SwordShieldStatus.h"
#include <fstream>
#include <sstream>

SwordShieldStatus SwordShieldStatus::StatusDataMAP;

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string SwordShieldStatus::uniqueStatusCsvFilePath = "Assets/CSV/WeaponUniqueStatus/SwordShieldStatus.csv";
bool SwordShieldStatus::IsLoadedUniqueStatusCSVFile = false;


void SwordShieldStatus::LoadUniqueStatusCSV()
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
		SwordShieldStatus statusData;
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


		//�V�[���h�̍ő�ϋv�l��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.maxShieldEnduranceValue = stoi(buf);

		//�V�[���h�̃m�b�N�o�b�N�ϐ����x����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.knockBackToleranceLevel = stoi(buf);
		

		//���݂̃V�[���h�̑ϋv�l��ݒ肷��
		statusData.SetCurrentShieldEnduranceValue(statusData.m_uniqueStatus.maxShieldEnduranceValue);

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP = statusData;

	}
	//���[�h����
	IsLoadedUniqueStatusCSVFile = true;
	return;
}

void SwordShieldStatus::InitUniqueStatus()
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
