#include "stdafx.h"
#include "EffectItemStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, EffectItemStatus> EffectItemStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string EffectItemStatus::effectItemStatusCsvFilePath = "Assets/CSV/EffectItemStatus.csv";

bool EffectItemStatus::IsLoadedEffectItemStatusCSVFile = false;


void EffectItemStatus::InitEffectItemStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedEffectItemStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadEffectItemStatusCSV(name);
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

void EffectItemStatus::LoadEffectItemStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(effectItemStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		EffectItemStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//���ʗʂ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.effectValue = stoi(buf);

		//�������Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.duration = stof(buf);
		
		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedEffectItemStatusCSVFile = true;
	return;
}
