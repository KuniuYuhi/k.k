#pragma once

/// <summary>
/// ステータスの構造体
/// </summary>
struct Status
{
private:
	int maxHp = 0;					//HPの最大値
	int hp = 0;						//HP
	int defaultAtk = 0;				//基礎攻撃力
	int atk = 0;					//攻撃力
	float dashSpeed = 0.0f;			//加速時のスピード
	float defaultSpeed = 0.0f;				//基礎スピード
	const char* characterName = nullptr;		//キャラクターの名前

	int maxEndurance = 0;		//耐久値の最大値
	int endurance = 0;		//耐久値。耐久値の概念が必要ないなら-1に設定する

	int skillAtk = 0;		//スキルの攻撃力

public:

	/// <summary>
	/// キャラクターの全てのステータスの初期化
	/// </summary>
	/// <param name="MaxHp"></param>
	/// <param name="Atk"></param>
	/// <param name="Speed"></param>
	/// <param name="name"></param>
	void InitCharacterStatus(int MaxHp,int Atk, float Speed,const char* name)
	{
		maxHp = MaxHp;
		hp = maxHp;
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
	void InitWeaponStatus(int Atk, int SkillAtk, int Endurance)
	{
		defaultAtk = Atk;
		atk = defaultAtk;
		skillAtk = SkillAtk;
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
	/// <summary>
	/// 最大HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return maxHp;
	}

	/// <summary>
	/// 攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		atk = setAtk;
	}
	/// <summary>
	/// 攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return atk;
	}

	/// <summary>
	/// スキルの攻撃力を設定
	/// </summary>
	/// <param name="setSkillAtk"></param>
	void SetSkillAtk(int setSkillAtk)
	{
		skillAtk = setSkillAtk;
	}
	/// <summary>
	/// スキルの攻撃力を取得
	/// </summary>
	/// <returns></returns>
	const int& GetSkillAtk() const
	{
		return skillAtk;
	}

	/// <summary>
	/// 基礎スピードの取得
	/// </summary>
	/// <returns></returns>
	const int& GetDefaultSpeed() const
	{
		return defaultSpeed;
	}
	/// <summary>
	/// ダッシュスピードの取得
	/// </summary>
	/// <returns></returns>
	const int& GetDashSpeed() const
	{
		return dashSpeed;
	}

	/// <summary>
	/// 耐久値の取得
	/// </summary>
	/// <returns></returns>
	const int& GetEndurance() const
	{
		return endurance;
	}
	/// <summary>
	/// 耐久値の最大値を取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxEndrance() const
	{
		return maxEndurance;
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

/// <summary>
/// 初期化。これを使ってステータスを初期化
/// </summary>
/// <param name="name">初期化したいキャラの名前</param>
void Init(const std::string& name);

private:
	/// <summary>
	/// csvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいキャラの名前</param>
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