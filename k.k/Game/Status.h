#pragma once
struct Status
{
private:
	int maxHp = 0;					//HPの最大値
	int hp = 0;					//HP
	int maxMp = 0;					//MPの最大値
	int mp = 0;					//MP
	int defaultAtk = 0;				//基礎攻撃力
	int atk = 0;					//攻撃力
	float dashSpeed = 0.0f;			//加速時のスピード
	float defaultSpeed = 0.0f;				//基礎スピード
	const char* characterName = nullptr;		//キャラクターの名前

	int maxEndurance = 0;		//耐久値の最大値
	int endurance = 0;		//耐久値

public:

	/// <summary>
	/// キャラクターの全てのステータスの初期化
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
	/// 武器の全てのステータスの初期化
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
	/// HPの設定
	/// </summary>
	/// <param name="setHp"></param>
	void SetHp(const int setHp)
	{
		hp = setHp;
	}
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetHp() const
	{
		return hp;
	}

	const int& GetMaxHp() const
	{
		return maxHp;
	}

	const int& GetMp() const
	{
		return mp;
	}

	const int& GetMaxMp() const
	{
		return maxMp;
	}

	void SetAtk(const int setAtk)
	{
		atk = setAtk;
	}

	const int& GetAtk() const
	{
		return atk;
	}

	const int& GetDefaultSpeed() const
	{
		return defaultSpeed;
	}
	const int& GetDashSpeed() const
	{
		return dashSpeed;
	}

	const int& GetEndurance() const
	{
		return endurance;
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

	/// <summary>
	/// MPの増減の計算。0以下になったら0になる。最大HPより大きくならない
	/// </summary>
	/// <param name="value">計算する値</param>
	/// <param name="UpOrDownFlag">足すか引くかのフラグ。trueで足す、falseで引く</param>
	void CalcMp(int value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//足し算
			AddMp(value);
		}
		else
		{
			//引き算
			SubMp(value);
		}
	}

	/// <summary>
	/// 耐久力の増減の計算
	/// </summary>
	/// <param name="value">計算する値</param>
	/// <param name="UpOrDownFlag">足すか引くかのフラグ。trueで足す、falseで引く</param>
	void CalcEndurance(int value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//足し算
			AddEndurance(value);
		}
		else
		{
			//引き算
			SubEndurance(value);
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

	/// <summary>
	/// MPを足す
	/// </summary>
	/// <param name="addMp"></param>
	void AddMp(float addMp)
	{
		mp += addMp;
		if (mp >= maxMp)
		{
			mp = maxMp;
		}
	}
	/// <summary>
	/// MPを引く
	/// </summary>
	/// <param name="subMp"></param>
	void SubMp(float subMp)
	{
		mp -= subMp;
		if (mp <= 0)
		{
			mp = 0;
		}
	}

	/// <summary>
	/// 耐久値を足す
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
	/// 耐久値を減らす
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