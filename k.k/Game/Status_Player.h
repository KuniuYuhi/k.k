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
		float currentStamina = 0;		//���݂̃X�^�~�i
		float defaultSpeed = 0.0f;		//��b�X�s�[�h
		float dashSpeed = 0.0f;			//�������̃X�s�[�h
		float evasionSpeed = 0.0f;		//������̃X�s�[�h
	};

	/// <summary>
	/// ����̃X�e�[�^�X
	/// </summary>
	struct WeaponStatus
	{
		int defaultAtk = 0;				//��b�U����
		int currentAtk = 0;				//���݂̍U����
		int skillAtk = 0;				//�X�L���̍U����
		int maxEndurance = 0;			//�ϋv�l�̍ő�l
		int currentEndurance = 0;				//�ϋv�l�B�ϋv�l�̊T�O���K�v�Ȃ��Ȃ�-1�ɐݒ肷��

		float comboKnockBackPower[3] = { 150.0f };	//�ʏ�U���̃R���{�ł̃m�b�N�o�b�N�p���[

		float skillKnockBackPower = 200.0f;

		//�x���Q�[�W�̂��Ƃ��l����

		//�^�C�v�@�a�@�ˁ@��
	};


	PlayerStatus m_playerStatus;
	WeaponStatus m_weaponStatus;

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
	/// ����̃X�e�[�^�X�\���̂��擾
	/// </summary>
	/// <returns></returns>
	const WeaponStatus& GetWeaponStatus()
	{
		return m_weaponStatus;
	}

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X��������
	/// </summary>
	/// <param name="name">�X�e�[�^�X���������������L�����̖��O</param>
	void InitPlayerStatus(const std::string& name);
	/// <summary>
	/// ����̃X�e�[�^�X�̏������B
	/// </summary>
	/// <param name="weaponName">�X�e�[�^�X������������������̖��O</param>
	void InitWeaponStatus(const std::string& weaponName);



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
	/// �v���C���[�̌��݂̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		m_playerStatus.currentPower = setAtk;
	}
	/// <summary>
	///  �v���C���[�̌��݂̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return m_playerStatus.currentPower;
	}
	/// <summary>
	/// �v���C���[�̌��݂̍U���͂𑝂₷
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


	///////////////////////////////////////////////////////////////////////////////////////////////
	///����̃X�e�[�^�X�Ɋւ���֐�
	///////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////
	///
	///////////////////////////////////////////////////////////////////////////////////////////////


		//csv�͎g��Ȃ�
	std::string name;          //���O

	//CSV�̏���ۑ������}�b�v
	static std::unordered_map<std::string, Status_Player> StatusDataMAP;
	//�v���C���[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string playerStatusCsvFilePath;
	static bool IsLoadedPlayerStatusCSVFile;
	//����̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string weaponStatusCsvFilePath;
	static bool IsLoadedWeaponStatusCSVFile;


	private:

	/// <summary>
	/// �v���C���[�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����v���C���[�̖��O</param>
	void LoadPlayerStatusCSV(const std::string& name);

	/// <summary>
	/// ����c�X�e�[�^�Xsv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂�������̖��O</param>
	void LoadWeaponStatusCSV(const std::string& name);



};

