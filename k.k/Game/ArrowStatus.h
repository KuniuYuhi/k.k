#pragma once

/// <summary>
/// ��̃X�e�[�^�X
/// </summary>
class ArrowStatus
{
private:

	struct Status
	{
		float arrowSpeed = 0.0f;			//��̃X�s�[�h
		Vector3 collisionSize = g_vec3Zero;	//�R���W�����̃T�C�Y
	};

	Status m_status;

public:

	Status& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ��̃X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�p�^�[���̖��O</param>
	void InitArrowStatus(const std::string& name);


	const float& GetArrowSpeed()
	{
		return m_status.arrowSpeed;
	}

	const Vector3& GetCollisionSize()
	{
		return m_status.collisionSize;
	}


	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, ArrowStatus> StatusDataMAP;
	//��̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string arrowStatusCsvFilePath;
	static bool IsLoadedArrowStatusCSVFile;

private:


	/// <summary>
	/// ��̃X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����p�^�[���̖��O</param>
	void LoadArrowStatusCSV(const std::string& name);



};

