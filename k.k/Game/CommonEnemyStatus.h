#pragma once


/// <summary>
/// �G�l�~�[�ɋ��ʂ���X�e�[�^�X
/// </summary>
class CommonEnemyStatus
{
private:

	/// <summary>
	/// �G�l�~�[�ŋ��ʂ���X�e�[�^�X
	/// </summary>
	struct EnemyStatus
	{
		int maxHp = 0;				//�ő�HP
		int currentHp = 0;			//���݂�HP
		int defaultPower = 0;		//��b�U����
		int currentPower = 0;		//���݂̍U����
		float defaultSpeed = 0;		//��b�X�s�[�h
		float attackIntarval = 0;	//�U���C���^�[�o��

		float approachDistance = 0.0f;	//�ڋ߂ł���ő�̋���
		float waitingDistance = 0.0f;	//�ҋ@���鋗��

	};


	EnemyStatus m_commonEnemyStatus;		//�G�l�~�[���ʂ̃X�e�[�^�X

public:

	/// <summary>
	/// �G�l�~�[�̋��ʃX�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X���������������L�����N�^�[�̖��O</param>
	void InitCommonEnemyStatus(const std::string& name);


	/// <summary>
	/// �G�l�~�[�̋��ʃX�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	const EnemyStatus& GetCommonEnemyStatus()
	{
		return m_commonEnemyStatus;
	}


	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="setHp"></param>
	void SetCurrentHp(const int setHp)
	{
		m_commonEnemyStatus.currentHp = setHp;
	}
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentHp() const
	{
		return m_commonEnemyStatus.currentHp;
	}
	/// <summary>
	/// �ő�HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return m_commonEnemyStatus.maxHp;
	}

	/// <summary>
	/// ���݂̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetCurrentAtk(const int setAtk)
	{
		m_commonEnemyStatus.currentPower = setAtk;
	}
	/// <summary>
	/// ���݂̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentAtk() const
	{
		return m_commonEnemyStatus.currentPower;
	}

	/// <summary>
	/// �X�s�[�h���擾
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return m_commonEnemyStatus.defaultSpeed;
	}

	/// <summary>
	/// �ڋ߂ł��鋗�����擾
	/// </summary>
	/// <returns></returns>
	const float& GetApproachDistance() const
	{
		return m_commonEnemyStatus.approachDistance;
	}
	/// <summary>
	/// �ҋ@���鋗�����擾
	/// </summary>
	/// <returns></returns>
	const float& GetWaitingDistance() const
	{
		return m_commonEnemyStatus.waitingDistance;
	}


	//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, CommonEnemyStatus> StatusDataMAP;
	//�G�l�~�[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string commonEnemyStatusCsvFilePath;
	static bool IsLoadedCommonEnemyStatusCSVFile;

private:

	/// <summary>
	/// �G�l�~�[�̋��ʃX�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����G�l�~�[�̖��O</param>
	void LoadCommonEnemyStatusCSV(const std::string& name);

};

