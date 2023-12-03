#pragma once
struct Status
{
	int maxHp = 0;					//HP�̍ő�l
	int hp = 0;					//HP
	int maxMp = 0;					//MP�̍ő�l
	int mp = 0;					//MP
	int defaultAtk = 0;				//��b�U����
	int atk = 0;					//�U����
	float dashSpeed = 0.0f;			//�������̃X�s�[�h
	float defaultSpeed = 0.0f;				//��b�X�s�[�h
	const char* characterName = nullptr;		//�L�����N�^�[�̖��O

	int maxEndurance = 0;		//�ϋv�l�̍ő�l
	int endurance = 0;		//�ϋv�l

	/// <summary>
	/// �L�����N�^�[�̑S�ẴX�e�[�^�X�̏�����
	/// </summary>
	/// <param name="MaxHp"></param>
	/// <param name="MaxMp"></param>
	/// <param name="Atk"></param>
	/// <param name="Speed"></param>
	/// <param name="name"></param>
	void InitCharacterStatus(int MaxHp,int MaxMp,int Atk, float Speed,const char* name)
	{
		maxHp = MaxHp;
		hp = maxHp;
		maxMp = MaxMp;
		mp = maxMp;
		defaultAtk = Atk;
		atk = defaultAtk;
		defaultSpeed = Speed;
		dashSpeed = defaultSpeed;
		characterName = name;
	}

	/// <summary>
	/// ����̑S�ẴX�e�[�^�X�̏�����
	/// </summary>
	/// <param name="Atk"></param>
	/// <param name="Endurance"></param>
	void InitWeaponStatus(int Atk, int Endurance)
	{
		defaultAtk = Atk;
		atk = defaultAtk = Atk;
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

static std::unordered_map<std::string, Status> StatusDataMAP;
static const std::string csvFilePath;
static bool IsLoadedCSVFile;

void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);

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