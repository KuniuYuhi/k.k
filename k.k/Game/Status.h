#pragma once


/// <summary>
/// ステータスの構造体
/// </summary>
struct Status
{
private:

	/// <summary>
	/// キャラクターの基本的なステータス
	/// </summary>
	struct CharacterStatus
	{
		int maxHp = 0;					//HPの最大値
		int hp = 0;						//HP
		int defaultAtk = 0;				//基礎攻撃力
		int atk = 0;					//攻撃力
		float dashSpeed = 0.0f;			//加速時のスピード
		float defaultSpeed = 0.0f;		//基礎スピード
		float maxSuperArmorPoint = 0;	//スーパーアーマーのポイント
		float superArmorPoint = 0;		//スーパーアーマーの最大値
		char rank = 'D';				//ランク。自身の強さの指標
	};

	/// <summary>
	/// 武器のステータス
	/// </summary>
	struct WeaponStatus
	{
		int defaultAtk = 0;				//基礎攻撃力
		int atk = 0;					//攻撃力
		int maxEndurance = 0;			//耐久値の最大値
		int endurance = 0;				//耐久値。耐久値の概念が必要ないなら-1に設定する
		int skillAtk = 0;				//スキルの攻撃力

		float comboKnockBackPower[3] = { 150.0f };	//通常攻撃のコンボでのノックバックパワー

		float skillKnockBackPower = 200.0f;
	};


	CharacterStatus characterStatus;
	WeaponStatus weaponStatus;

public:
	/// <summary>
	/// キャラクターのステータス構造体を取得
	/// </summary>
	/// <returns></returns>
	const CharacterStatus& GetCharacterStatus()
	{
		return characterStatus;
	}
	/// <summary>
	/// 武器のステータス構造体を取得
	/// </summary>
	/// <returns></returns>
	const WeaponStatus& GetWeaponStatus()
	{
		return weaponStatus;
	}

	/// <summary>
	/// キャラクターのステータスの初期化。これを使ってステータスを初期化
	/// </summary>
	/// <param name="name">初期化したいキャラの名前</param>
	void InitCharacterStatus(const std::string& name);
	/// <summary>
	/// 武器のステータスの初期化。
	/// </summary>
	/// <param name="weaponName"></param>
	void InitWeaponStatus(const std::string& weaponName);

	/// <summary>
	/// 武器の全てのステータスの初期化
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
	/// HPの設定
	/// </summary>
	/// <param name="setHp"></param>
	void SetHp(const int setHp)
	{
		characterStatus.hp = setHp;
	}
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetHp() const
	{
		return characterStatus.hp;
	}
	/// <summary>
	/// 最大HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return characterStatus.maxHp;
	}

	/// <summary>
	/// スーパーアーマーのポイントを設定
	/// </summary>
	/// <param name="setSuperArmor"></param>
	void SetSuperArmorPoint(float setSuperArmor)
	{
		characterStatus.superArmorPoint = setSuperArmor;
	}
	/// <summary>
	/// スーパーアーマーのポイントを取得
	/// </summary>
	/// <returns></returns>
	const float& GetSuperArmorPoint() const
	{
		return characterStatus.superArmorPoint;
	}
	/// <summary>
	/// スーパーアーマーの最大ポイントを取得
	/// </summary>
	/// <returns></returns>
	const float& GetMaxSuperArmorPoint() const
	{
		return characterStatus.maxSuperArmorPoint;
	}


	/// <summary>
	/// キャラクターの攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetAtk(const int setAtk)
	{
		characterStatus.atk = setAtk;
	}
	/// <summary>
	/// キャラクターの攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return characterStatus.atk;
	}

	/// <summary>
	/// 武器の攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetWeaponAtk(const int setAtk)
	{
		weaponStatus.atk = setAtk;
	}
	/// <summary>
	/// 武器の攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetWeaponAtk() const
	{
		return weaponStatus.atk;
	}

	/// <summary>
	/// スキルの攻撃力を設定
	/// </summary>
	/// <param name="setSkillAtk"></param>
	void SetSkillAtk(int setSkillAtk)
	{
		weaponStatus.skillAtk = setSkillAtk;
	}
	/// <summary>
	/// スキルの攻撃力を取得
	/// </summary>
	/// <returns></returns>
	const int& GetSkillAtk() const
	{
		return weaponStatus.skillAtk;
	}

	/// <summary>
	/// 基礎スピードの取得
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return characterStatus.defaultSpeed;
	}
	/// <summary>
	/// ダッシュスピードの取得
	/// </summary>
	/// <returns></returns>
	const float& GetDashSpeed() const
	{
		return characterStatus.dashSpeed;
	}

	/// <summary>
	/// 耐久値の取得
	/// </summary>
	/// <returns></returns>
	const int& GetEndurance() const
	{
		return weaponStatus.endurance;
	}
	/// <summary>
	/// 耐久値の最大値を取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxEndrance() const
	{
		return weaponStatus.maxEndurance;
	}

	/// <summary>
	/// キャラクターのランクを取得
	/// </summary>
	/// <returns></returns>
	const char& GetRank() const
	{
		return characterStatus.rank;
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

	/// <summary>
	/// スーパーアーマーのポイントの増減の計算
	/// </summary>
	/// <param name="value">計算する値</param>
	/// <param name="UpOrDownFlag">足すか引くかのフラグ。trueで足す、falseで引く</param>
	/// <returns>スーパーアーマーが割れたらtrue、割れてないならfalse</returns>
	bool CalcSuperArmorPoint(float value, bool UpOrDownFlag)
	{
		if (UpOrDownFlag == true)
		{
			//足し算
			AddSuperArmorPoint(value);
			//スーパーアーマーが完全に回復した瞬間だったら
			if (characterStatus.superArmorPoint == 
				characterStatus.maxSuperArmorPoint)
			{
				//ブレイクしていない状態を返す
				return false;
			}

			//ブレイクしているのでtrueを返す
			return true;
		}
		else
		{
			//引き算
			SubSuperArmorPoint(value);
			//スーパーアーマーがブレイクした瞬間なら
			if (characterStatus.superArmorPoint == 0)
			{
				//ブレイクしているのでtrueを返す
				return true;
			}

			//ブレイクしていないのでfalseを返す
			return false;
			
		}
	}

	//csvは使わない
std::string name;          //名前

//CSVの情報を保存したマップ
static std::unordered_map<std::string, Status> StatusDataMAP;
//キャラクターのステータスの初期化で使う静的な型
static const std::string csvFilePath;
static bool IsLoadedCSVFile;
//武器のステータスの初期化で使う静的な型
static const std::string weaponCsvFilePath;
static bool IsLoadedWeaponCSVFile;

private:
	/// <summary>
	/// csvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいキャラの名前</param>
	void LoadCSV(const std::string& name);

	/// <summary>
	/// csvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいキャラの名前</param>
	void LoadWeaponCSV(const std::string& name);

	/// <summary>
	/// HPを足す
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
	/// HPを引く
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
	/// 耐久値を足す
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
	/// 耐久値を減らす
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
	/// スーパーアーマーのポイントを足す
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
	/// スーパーアーマーのポイントを減らす
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