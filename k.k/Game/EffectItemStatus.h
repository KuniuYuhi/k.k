#pragma once

/// <summary>
/// ���炩�̌��ʕt�^�A�C�e��
/// </summary>
class EffectItemStatus
{
private:

	struct Status
	{
		int effectValue = 0;			//���ʗ�
		float duration = 0.0f;			//��������
	};

	Status m_status;

public:

	Status GetStatus()
	{
		return m_status;
	}


	/// <summary>
	/// �G�t�F�N�g�A�C�e���̃X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�G�t�F�N�g�A�C�e�����������������A�C�e���̖��O</param>
	void InitEffectItemStatus(const std::string& name);



	const int& GetEffectValue()
	{
		return m_status.effectValue;
	}

	const float& GetDuration()
	{
		return m_status.duration;
	}



	//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, EffectItemStatus> StatusDataMAP;
	//�G�t�F�N�g�A�C�e���̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string effectItemStatusCsvFilePath;
	static bool IsLoadedEffectItemStatusCSVFile;

public:

	/// <summary>
	/// �G�t�F�N�g�A�C�e���X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����A�C�e���̖��O</param>
	void LoadEffectItemStatusCSV(const std::string& name);

};

