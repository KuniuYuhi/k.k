#include "stdafx.h"
#include "Status.h"
#include <fstream>
#include <sstream>

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string Status::csvFilePath = "Assets/CSV/Status.csv";
const std::string Status::weaponCsvFilePath= "Assets/CSV/WeaponStatus.csv";

std::unordered_map<std::string, Status> Status::StatusDataMAP = {};
bool Status::IsLoadedCSVFile = false;
bool Status::IsLoadedWeaponCSVFile = false;



void Status::InitCharacterStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedCSVFile == false) 
	{
		//CSV�t�@�C����ǂݍ���
		LoadCSV(name);
	}
	//�L�[��name�ł���v�f��T��
	auto itr = StatusDataMAP.find(name);
	//���O���Ȃ�
	if (itr == StatusDataMAP.end())
	{
		return;
	}
	//�����Ɠ������O�̃f�[�^����
	*this = itr->second;

	return;
}

void Status::InitWeaponStatus(const std::string& weaponName)
{
	//�����CSV�t�@�C����ǂݍ���ł��Ȃ��Ȃ�
	if (IsLoadedWeaponCSVFile == false) 
	{
		//�����CSV�t�@�C����ǂݍ���
		LoadWeaponCSV(weaponName);
	}
	//�L�[��name�ł���v�f��T��
	auto itr = StatusDataMAP.find(weaponName);
	//���O���Ȃ�
	if (itr == StatusDataMAP.end())
	{
		return;
	}
	//�����Ɠ������O�̃f�[�^����
	*this = itr->second;

	return;
}













void Status::LoadCSV(const std::string& name) {

	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(csvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		Status statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//HP��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.characterStatus.maxHp = stoi(buf);
		//Attack��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.characterStatus.defaultAtk = stoi(buf);
		//Speed��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.characterStatus.defaultSpeed = stoi(buf);
		//�X�[�p�[�A�[�}�[��ǂݍ��ށB�{�X�ȊO��-1
		getline(i_stream, buf, ',');
		statusData.characterStatus.maxSuperArmorPoint = stoi(buf);



		//HP��ݒ�
		statusData.characterStatus.hp = statusData.characterStatus.maxHp;
		//�U���͂�ݒ�
		statusData.characterStatus.atk = statusData.characterStatus.defaultAtk;
		//�X�[�p�[�A�[�}�[�̒l��ݒ�
		statusData.characterStatus.superArmorPoint = statusData.characterStatus.maxSuperArmorPoint;

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedCSVFile = true;
	return;
}

void Status::LoadWeaponCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;

	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(weaponCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		Status statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//�U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.weaponStatus.defaultAtk = stoi(buf);
		//�ϋv�͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.weaponStatus.maxEndurance = stoi(buf);
		//�X�L���U���͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.weaponStatus.skillAtk = stoi(buf);

		for (int i = 0; i < 3; i++)
		{
			//�ʏ�U���̃m�b�N�o�b�N�p���[��ǂݍ��ށB
			getline(i_stream, buf, ',');
			statusData.weaponStatus.comboKnockBackPower[i] = stoi(buf);
		}

		//�X�L���̃m�b�N�o�b�N�p���[��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.weaponStatus.skillKnockBackPower = stoi(buf);

		//�U���͂�ݒ�
		statusData.weaponStatus.atk = statusData.weaponStatus.defaultAtk;
		//�ϋv�͂�ݒ�
		statusData.weaponStatus.endurance = statusData.weaponStatus.maxEndurance;

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedWeaponCSVFile = true;
	return;
}
