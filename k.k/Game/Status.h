#pragma once

/// <summary>
/// �X�e�[�^�X�̍\����
/// </summary>
struct Status
{
private:

	/// <summary>
	/// �L�����N�^�[�̊�{�I�ȃX�e�[�^�X
	/// </summary>
	struct CharacterStatus
	{
		int maxHp = 0;					//HP�̍ő�l
		int hp = 0;						//HP
		int defaultAtk = 0;				//��b�U����
		int atk = 0;					//�U����
		float dashSpeed = 0.0f;			//�������̃X�s�[�h
		float defaultSpeed = 0.0f;		//��b�X�s�[�h
	};

	/// <summary>
	/// �S�ẴL�����N�^�[�ɂ͂Ȃ��ǉ��̃X�e�[�^�X
	/// </summary>
	struct ExtraStatus
	{
		float maxSuperArmorPoint = 0;	//�X�[�p�[�A�[�}�[�̃|�C���g
		float superArmorPoint = 0;		//�X�[�p�[�A�[�}�[�̍ő�l

	};

	/// <summary>
	/// ����̃X�e�[�^�X
	/// </summary>
	struct WeaponStatus
	{
		int defaultAtk = 0;				//��b�U����
		int atk = 0;					//�U����
		int maxEndurance = 0;			//�ϋv�l�̍ő�l
		int endurance = 0;				//�ϋv�l�B�ϋv�l�̊T�O���K�v�Ȃ��Ȃ�-1�ɐݒ肷��
		int skillAtk = 0;				//�X�L���̍U����

		float comboKnockBackPower[3] = { 150.0f };	//�ʏ�U���̃R���{�ł̃m�b�N�o�b�N�p���[

		float skillKnockBackPower = 200.0f;
	};


	CharacterStatus characterStatus;
	ExtraStatus extraStatus;
	WeaponStatus weaponStatus;


	int maxHp = 0;					//HP�̍ő�l
	int hp = 0;						//HP
	int defaultAtk = 0;				//��b�U����
	int atk = 0;					//�U����
	float dashSpeed = 0.0f;			//�������̃X�s�[�h
	float defaultSpeed = 0.0f;				//��b�X�s�[�h
	const char* characterName = nullptr;		//�L�����N�^�[�̖��O

	int maxEndurance = 0;		//�ϋv�l�̍ő�l
	int endurance = 0;		//�ϋv�l�B�ϋv�l�̊T�O���K�v�Ȃ��Ȃ�-1�ɐݒ肷��

	int skillAtk = 0;		//�X�L���̍U����

public:
	/// <summary>
	/// �L�����N�^�[�̃X�e�[�^�X�\���̂��擾
	/// </summary>
	/// <returns></returns>
	const CharacterStatus& GetCharacterStatus()
	{
		return characterStatus;
	}
	/// <summary>
	/// ����̃X�e�[�^�X�\���̂��擾
	/// </summary>
	/// <returns></returns>
	const WeaponStatus& GetWeaponStatus()
	{
		return weaponStatus;
	}

	/// <summary>
	/// �L�����N�^�[�̃X�e�[�^�X�̏������B������g���ăX�e�[�^�X��������
	/// </summary>
	/// <param name="name">�������������L�����̖��O</param>
	void InitCharacterStatus(const std::string& name);
	/// <summary>
	/// ����̃X�e�[�^�X�̏������B
	/// </summary>
	/// <param name="weaponName"></param>
	void InitWeaponStatus(const std::string& weaponName);

	/// <summary>
	/// ����̑S�ẴX�e�[�^�X�̏�����
	/// </summary>
	/// <param name="Atk"></param>
	/// <param name="Endurance"></param>
	void InitWeaponStatus(int Atk, int SkillAtk, int Endurance)
	{
		defaultAtk = Atk;
		atk = defaultAtk;
		skillAtk = SkillAtk;
		maxEndurance = Endurance;
		endurance = maxEndurance;
	}

	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="setHp"></param>
	void SetHp(const int setHp)
	{
		hp = setHp;
	}
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetHp() const
	{
		return hp;
	}
	/// <summary>
	/// �ő�HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return maxHp;
	}

	/// <summary>
	/// �U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		atk = setAtk;
	}
	/// <summary>
	/// �U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return atk;
	}

	/// <summary>
	/// �X�L���̍U���͂�ݒ�
	/// </summary>
	/// <param name="setSkillAtk"></param>
	void SetSkillAtk(int setSkillAtk)
	{
		skillAtk = setSkillAtk;
	}
	/// <summary>
	/// �X�L���̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	const int& GetSkillAtk() const
	{
		return skillAtk;
	}

	/// <summary>
	/// ��b�X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetDefaultSpeed() const
	{
		return defaultSpeed;
	}
	/// <summary>
	/// �_�b�V���X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetDashSpeed() const
	{
		return dashSpeed;
	}

	/// <summary>
	/// �ϋv�l�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetEndurance() const
	{
		return endurance;
	}
	/// <summary>
	/// �ϋv�l�̍ő�l���擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxEndrance() const
	{
		return maxEndurance;
	}


	/// <summary>
	/// HP�̑����̌v�Z�B0�ȉ��ɂȂ�����0�ɂȂ�B�ő�HP���傫���Ȃ�Ȃ�
	/// </summary>
	/// <param name="value">�v�Z����l</param>
	/// <param name="UpOrDownFlag">�������������̃t���O�Btrue�ő����Afalse�ň���</param>
	void CalcHp(int value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//�����Z
			AddHp(value);
		}
		else
		{
			//�����Z
			SubHp(value);
		}
	}

	/// <summary>
	/// �ϋv�͂̑����̌v�Z
	/// </summary>
	/// <param name="value">�v�Z����l</param>
	/// <param name="UpOrDownFlag">�������������̃t���O�Btrue�ő����Afalse�ň���</param>
	void CalcEndurance(int value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//�����Z
			AddEndurance(value);
		}
		else
		{
			//�����Z
			SubEndurance(value);
		}
	}

	//csv�͎g��Ȃ�
std::string name;          //���O

//CSV�̏���ۑ������}�b�v
static std::unordered_map<std::string, Status> StatusDataMAP;
//�L�����N�^�[�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
static const std::string csvFilePath;
static bool IsLoadedCSVFile;
//����̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
static const std::string weaponCsvFilePath;
static bool IsLoadedWeaponCSVFile;

private:
	/// <summary>
	/// csv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����L�����̖��O</param>
	void LoadCSV(const std::string& name);

	/// <summary>
	/// csv�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="name">�ǂݍ��݂����L�����̖��O</param>
	void LoadWeaponCSV(const std::string& name);

	/// <summary>
	/// HP�𑫂�
	/// </summary>
	/// <param name="addHp"></param>
	void AddHp(int addHp)
	{
		hp += addHp;
		if (hp >= maxHp)
		{
			hp = maxHp;
		}
	}
	/// <summary>
	/// HP������
	/// </summary>
	/// <param name="subHp"></param>
	void SubHp(int subHp)
	{
		hp -= subHp;
		if (hp <= 0)
		{
			hp = 0;
		}
	}

	/// <summary>
	/// �ϋv�l�𑫂�
	/// </summary>
	/// <param name="addEndurance"></param>
	void AddEndurance(int addEndurance)
	{
		endurance += addEndurance;
		if (endurance >= maxEndurance)
		{
			endurance = maxEndurance;
		}
	}
	/// <summary>
	/// �ϋv�l�����炷
	/// </summary>
	/// <param name="subEndurance"></param>
	void SubEndurance(int subEndurance)
	{
		endurance -= subEndurance;
		if (endurance <= 0)
		{
			endurance = 0;
		}
	}

};