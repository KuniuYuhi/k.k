#pragma once
struct Status
{
	int maxHp = 0;
	int hp = 0;
	int maxMp = 0;
	int mp = 0;
	int defaultAtk = 0;
	int atk = 0;
	float dashSpeed = 0.0f;
	float defaultSpeed = 0.0f;
	const char* characterName = nullptr;

	/// <summary>
	/// �S�ẴX�e�[�^�X�̏�����
	/// </summary>
	/// <param name="MaxHp"></param>
	/// <param name="MaxMp"></param>
	/// <param name="Atk"></param>
	/// <param name="Speed"></param>
	/// <param name="name"></param>
	void InitStatus(int MaxHp,int MaxMp,int Atk, float Speed,const char* name)
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
};