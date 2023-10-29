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
	/// 全てのステータスの初期化
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
	/// HPの設定
	/// </summary>
	/// <param name="setHp"></param>
	void SetHp(const int setHp)
	{
		hp = setHp;
	}
	/// <summary>
	/// HPの増減の計算。0以下になったら0になる。最大HPより大きくならない
	/// </summary>
	/// <param name="value">計算する値</param>
	/// <param name="UpOrDownFlag">足すか引くかのフラグ。trueで足す、falseで引く</param>
	void CalcHp(int value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//足し算
			AddHp(value);
		}
		else
		{
			//引き算
			SubHp(value);
		}
	}

	//csvは使わない
	std::string name;          //名前

static std::unordered_map<std::string, Status> StatusDataMAP;
static const std::string csvFilePath;
static bool IsLoadedCSVFile;

void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);

	/// <summary>
	/// HPを足す
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
	/// HPを引く
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