#include "stdafx.h"
#include "KnockBackStatus.h"
#include <fstream>
#include <sstream>


KnockBackStatus KnockBackStatus::StatusDataMAP;

//�X�e�[�^�X���L�ڂ����t�@�C���p�X
const std::string KnockBackStatus::knockBackStatusCsvFilePath = "Assets/CSV/KnockBackStatus.csv";
bool KnockBackStatus::IsLoadedKnockBackStatusCSVFile = false;


void KnockBackStatus::InitKnockBackStatus()
{
	//CSV�t�@�C�������[�h���Ă��Ȃ��Ȃ�
	if (IsLoadedKnockBackStatusCSVFile == false)
	{
		//CSV�t�@�C����ǂݍ���
		LoadKnockBackStatusCSV();
	}

	//�����Ɠ������O�̃f�[�^����
	*this = StatusDataMAP;

	return;
}

void KnockBackStatus::LoadKnockBackStatusCSV()
{
	//�ǂݍ���csv�t�@�C���̃f�[�^���i�[����string�^
	std::string str_buf;
	//�u,�v�܂ł�ǂݍ���Ŋi�[����string�^
	std::string buf;
	// �ǂݍ���csv�t�@�C�����J��(std::ifstream�̃R���X�g���N�^�ŊJ��)
	std::ifstream csvFile(knockBackStatusCsvFilePath);

	//�ŏ���1�s�͔�΂��B
	getline(csvFile, str_buf);


	//�C���X�^���X
	KnockBackStatus statusData;
	
	//�z��̔ԍ�
	int num = 0;

	// getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e��str_buf�Ɋi�[)
	while (getline(csvFile, str_buf))
	{
		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::istringstream i_stream(str_buf);

		//�p�^�[���̖��O�͔�΂�
		getline(i_stream, buf, ',');

		//�m�b�N�o�b�N���x����ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status[num].knockBackLevel = stoi(buf);

		//�����x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status[num].initialVelocity = stof(buf);

		//�p�x��ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status[num].angle = stof(buf);


		//�������Ԃ�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status[num].duration = stof(buf);

		//�d�͂�ǂݍ��ށB
		getline(i_stream, buf, ',');
		statusData.m_status[num].gravity = stof(buf);

		// �R���W�����̃T�C�Y��ǂݍ���
		if (i_stream.peek() == '"') {
			// �擪��"�̏ꍇ�A���̒��̃e�L�X�g�����ׂēǂݎ��
			i_stream.get(); // �ŏ���"����菜��
			getline(i_stream, buf, '"'); // "�܂ł�ǂݍ���
			i_stream.get(); // �Ō��"����菜��
		}

		// �m�b�N�o�b�N�̃X�s�[�h���R���}�ŕ������A���ꂼ��̒l��ݒ肷��
		std::replace(buf.begin(), buf.end(), ',', ' ');

		std::istringstream sizeStream(buf);

		sizeStream >> statusData.m_status[num].knockBackTimeScale.x
			>> statusData.m_status[num].knockBackTimeScale.y;

		//���̔z��ɂ���
		num++;

	}


	//�}�b�v�Ɏ擾�����f�[�^����
	StatusDataMAP = statusData;

	//���[�h����
	IsLoadedKnockBackStatusCSVFile = true;
	return;
}


