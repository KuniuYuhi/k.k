#pragma once



/// <summary>
/// �v���C���[�̃X�e�[�^�X
/// </summary>
class Status_Player
{
private:

	/// <summary>
	/// �L�����N�^�[�̊�{�I�ȃX�e�[�^�X
	/// </summary>
	struct PlayerStatus
	{
		int maxHp = 0;					//HP�̍ő�l
		int currentHp = 0;				//���݂�HP
		int defaultPower = 0;				//��b�U����
		int currentPower = 0;				//���݂̍U����
		float maxStamina = 0;			//�X�^�~�i�̍ő�l
		float staminaRecoveryRate = 0.0f;		//�X�^�~�i�̉񕜑��x�{��
		float currentStamina = 0;		//���݂̃X�^�~�i
		float defaultSpeed = 0.0f;		//��b�X�s�[�h
		float dashSpeed = 0.0f;			//�������̃X�s�[�h
		float evasionSpeed = 0.0f;		//������̃X�s�[�h
	};


	PlayerStatus m_playerStatus;

public:

	/// <summary>
	/// �L�����N�^�[�̃X�e�[�^�X�\���̂��擾
	/// </summary>
	/// <returns></returns>
	const PlayerStatus& GetCharacterStatus()
	{
		return m_playerStatus;
	}
	
	/// <summary>
	/// �v���C���[�̃X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X���������������L�����̖��O</param>
	void InitPlayerStatus(const std::string& name);


	//////////////////////////////////////////////////////////////////////////////////////////////
	///�v���C���[�̃X�e�[�^�X�Ɋւ���֐�
	////////////////////////////////////////////////////////////////////////////////////////////// 
	
	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="setHp"></param>
	void SetCurrentHp(const int setHp)
	{
		m_playerStatus.currentHp = setHp;
	}
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetHp() const
	{
		return m_playerStatus.currentHp;
	}
	/// <summary>
	/// �ő�HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return m_playerStatus.maxHp;
	}

	/// <summary>
	/// ���݂̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		m_playerStatus.currentPower = setAtk;
	}
	/// <summary>
	/// ���݂̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return m_playerStatus.currentPower;
	}
	/// <summary>
	/// ���݂̍U���͂𑝂₷
	/// </summary>
	/// <param name="addPower">���Z����U����</param>
	void AddPowerToCurrentPower(int addPower)
	{
		m_playerStatus.currentPower += addPower;
	}

	/// <summary>
	/// ���݂̃X�^�~�i���擾
	/// </summary>
	/// <returns></returns>
	const float& GetCurrentStamina() const
	{
		return m_playerStatus.currentStamina;
	}
	
	/// <summary>
	/// �X�^�~�i�̉񕜑��x�{�����擾
	/// </summary>
	/// <returns></returns>
	float GetStaminaRecoveryRate()
	{
		return m_playerStatus.staminaRecoveryRate;
	}

	/// <summary>
	/// ��b�X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return m_playerStatus.defaultSpeed;
	}
	/// <summary>
	/// �_�b�V���X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetDashSpeed() const
	{
		return m_playerStatus.dashSpeed;
	}


	///////////////////////////////////////////////////////////
	///
	///////////////////////////////////////////////////////////

	/// <summary>
	/// �X�^�~�i������ł��邩�B�ł���Ƃ��͏����B�ł��Ȃ��Ƃ��͏���Ȃ�
	/// </summary>
	/// <param name="consumeValue">������</param>
	/// <returns>����ł�����true�A����ł��Ȃ�������false</returns>
	bool TryConsumeStamina(float consumeValue);

	/// <summary>
	/// �X�^�~�i������ł��邩�`�F�b�N����B�X�^�~�i�͏���Ȃ�
	/// </summary>
	/// <param name="checkValue">���ׂ����l</param>
	/// <returns>����ł���Ȃ�true</returns>
	bool CheckConsumeStamina(float checkValue);

	/// <summary>
	/// �X�^�~�i���񕜂���
	/// </summary>
	/// <param name="recoveryValue">�񕜂����</param>
	void RecoveryStamina(float recoveryValue);






	///////////////////////////////////////////////////////////

		//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, Status_Player> StatusDataMAP;
	//�v���C���[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string playerStatusCsvFilePath;
	static bool IsLoadedPlayerStatusCSVFile;


private:

	/// <summary>
	/// �v���C���[�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����v���C���[�̖��O</param>
	void LoadPlayerStatusCSV(const std::string& name);


};

