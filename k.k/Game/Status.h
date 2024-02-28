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
		float maxSuperArmorPoint = 0;	//�X�[�p�[�A�[�}�[�̃|�C���g
		float superArmorPoint = 0;		//�X�[�p�[�A�[�}�[�̍ő�l
		char rank = 'D';				//�����N�B���g�̋����̎w�W
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
	WeaponStatus weaponStatus;

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
		weaponStatus.defaultAtk = Atk;
		weaponStatus.atk = weaponStatus.defaultAtk;
		weaponStatus.skillAtk = SkillAtk;
		weaponStatus.maxEndurance = Endurance;
		weaponStatus.endurance = weaponStatus.maxEndurance;
	}

	/// <summary>
	/// HP�̐ݒ�
	/// </summary>
	/// <param name="setHp"></param>
	void SetHp(const int setHp)
	{
		characterStatus.hp = setHp;
	}
	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetHp() const
	{
		return characterStatus.hp;
	}
	/// <summary>
	/// �ő�HP�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return characterStatus.maxHp;
	}

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̃|�C���g��ݒ�
	/// </summary>
	/// <param name="setSuperArmor"></param>
	void SetSuperArmorPoint(float setSuperArmor)
	{
		characterStatus.superArmorPoint = setSuperArmor;
	}
	/// <summary>
	/// �X�[�p�[�A�[�}�[�̃|�C���g���擾
	/// </summary>
	/// <returns></returns>
	const float& GetSuperArmorPoint() const
	{
		return characterStatus.superArmorPoint;
	}
	/// <summary>
	/// �X�[�p�[�A�[�}�[�̍ő�|�C���g���擾
	/// </summary>
	/// <returns></returns>
	const float& GetMaxSuperArmorPoint() const
	{
		return characterStatus.maxSuperArmorPoint;
	}


	/// <summary>
	/// �L�����N�^�[�̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		characterStatus.atk = setAtk;
	}
	/// <summary>
	/// �L�����N�^�[�̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return characterStatus.atk;
	}

	/// <summary>
	/// ����̍U���͂̐ݒ�
	/// </summary>
	/// <param name="setAtk"></param>
	void SetWeaponAtk(const int setAtk)
	{
		weaponStatus.atk = setAtk;
	}
	/// <summary>
	/// ����̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetWeaponAtk() const
	{
		return weaponStatus.atk;
	}

	/// <summary>
	/// �X�L���̍U���͂�ݒ�
	/// </summary>
	/// <param name="setSkillAtk"></param>
	void SetSkillAtk(int setSkillAtk)
	{
		weaponStatus.skillAtk = setSkillAtk;
	}
	/// <summary>
	/// �X�L���̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	const int& GetSkillAtk() const
	{
		return weaponStatus.skillAtk;
	}

	/// <summary>
	/// ��b�X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return characterStatus.defaultSpeed;
	}
	/// <summary>
	/// �_�b�V���X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetDashSpeed() const
	{
		return characterStatus.dashSpeed;
	}

	/// <summary>
	/// �ϋv�l�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetEndurance() const
	{
		return weaponStatus.endurance;
	}
	/// <summary>
	/// �ϋv�l�̍ő�l���擾
	/// </summary>
	/// <returns></returns>
	const int& GetMaxEndrance() const
	{
		return weaponStatus.maxEndurance;
	}

	/// <summary>
	/// �L�����N�^�[�̃����N���擾
	/// </summary>
	/// <returns></returns>
	const char& GetRank() const
	{
		return characterStatus.rank;
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

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̃|�C���g�̑����̌v�Z
	/// </summary>
	/// <param name="value">�v�Z����l</param>
	/// <param name="UpOrDownFlag">�������������̃t���O�Btrue�ő����Afalse�ň���</param>
	/// <returns>�X�[�p�[�A�[�}�[�����ꂽ��true�A����ĂȂ��Ȃ�false</returns>
	bool CalcSuperArmorPoint(float value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//�����Z
			AddSuperArmorPoint(value);
			//�X�[�p�[�A�[�}�[�����S�ɉ񕜂����u�Ԃ�������
			if (characterStatus.superArmorPoint == 
				characterStatus.maxSuperArmorPoint)
			{
				//�u���C�N���Ă��Ȃ���Ԃ�Ԃ�
				return false;
			}

			//�u���C�N���Ă���̂�true��Ԃ�
			return true;
		}
		else
		{
			//�����Z
			SubSuperArmorPoint(value);
			//�X�[�p�[�A�[�}�[���u���C�N�����u�ԂȂ�
			if (characterStatus.superArmorPoint == 0)
			{
				//�u���C�N���Ă���̂�true��Ԃ�
				return true;
			}

			//�u���C�N���Ă��Ȃ��̂�false��Ԃ�
			return false;
			
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
		characterStatus.hp += addHp;
		if (characterStatus.hp >= characterStatus.maxHp)
		{
			characterStatus.hp = characterStatus.maxHp;
		}
	}
	/// <summary>
	/// HP������
	/// </summary>
	/// <param name="subHp"></param>
	void SubHp(int subHp)
	{
		characterStatus.hp -= subHp;
		if (characterStatus.hp <= 0)
		{
			characterStatus.hp = 0;
		}
	}

	/// <summary>
	/// �ϋv�l�𑫂�
	/// </summary>
	/// <param name="addEndurance"></param>
	void AddEndurance(int addEndurance)
	{
		weaponStatus.endurance += addEndurance;
		if (weaponStatus.endurance >= weaponStatus.maxEndurance)
		{
			weaponStatus.endurance = weaponStatus.maxEndurance;
		}
	}
	/// <summary>
	/// �ϋv�l�����炷
	/// </summary>
	/// <param name="subEndurance"></param>
	void SubEndurance(int subEndurance)
	{
		weaponStatus.endurance -= subEndurance;
		if (weaponStatus.endurance <= 0)
		{
			weaponStatus.endurance = 0;
		}
	}

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̃|�C���g�𑫂�
	/// </summary>
	/// <param name="addEndurance"></param>
	void AddSuperArmorPoint(float addSuperArmorPoint)
	{
		characterStatus.superArmorPoint += addSuperArmorPoint;
		if (characterStatus.superArmorPoint >= characterStatus.maxSuperArmorPoint)
		{
			characterStatus.superArmorPoint = characterStatus.maxSuperArmorPoint;
		}
	}
	/// <summary>
	/// �X�[�p�[�A�[�}�[�̃|�C���g�����炷
	/// </summary>
	/// <param name="subEndurance"></param>
	void SubSuperArmorPoint(float subSuperArmorPoint)
	{
		characterStatus.superArmorPoint -= subSuperArmorPoint;
		if (characterStatus.superArmorPoint <= 0)
		{
			characterStatus.superArmorPoint = 0;
		}
	}

};