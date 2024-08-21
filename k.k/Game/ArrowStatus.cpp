#include "stdafx.h"
#include "ArrowStatus.h"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, ArrowStatus> ArrowStatus::StatusDataMAP = {};
//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string ArrowStatus::arrowStatusCsvFilePath = "Assets/CSV/WeaponUniqueStatus/ArrowStatus.csv";

bool ArrowStatus::IsLoadedArrowStatusCSVFile = false;


void ArrowStatus::InitArrowStatus(const std::string& name)
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedArrowStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadArrowStatusCSV(name);
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

void ArrowStatus::LoadArrowStatusCSV(const std::string& name)
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(arrowStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);
	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		ArrowStatus statusData;
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);
		//���O��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//��̃X�s�[�h��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status.arrowSpeed = stof(buf);
	
		// �R���W�����̃T�C�Y��ǂݍ���
		if (i_stream.peek() == '"') {
			// �擪��"�̏ꍇ�A���̒��̃e�L�X�g�����ׂēǂݎ��
			i_stream.get(); // �ŏ���"����菜��
			getline(i_stream, buf, '"'); // "�܂ł�ǂݍ���
			i_stream.get(); // �Ō��"����菜��
		}

		// �R���W�����T�C�Y���R���}�ŕ������A���ꂼ��̒l��ݒ肷��
		std::replace(buf.begin(), buf.end(), ',', ' ');

		std::istringstream sizeStream(buf);

		sizeStream >> statusData.m_status.collisionSize.x
			>> statusData.m_status.collisionSize.y
			>> statusData.m_status.collisionSize.z;

		//�}�b�v�Ɏ擾�����f�[�^����
		StatusDataMAP[statusData.name] = statusData;

	}
	//���[�h����
	IsLoadedArrowStatusCSVFile = true;
	return;
}
