#pragma once



/// <summary>
/// プレイヤーのステータス
/// </summary>
class Status_Player
{
private:

	/// <summary>
	/// キャラクターの基本的なステータス
	/// </summary>
	struct PlayerStatus
	{
		int maxHp = 0;					//HPの最大値
		int currentHp = 0;				//現在のHP
		int defaultPower = 0;				//基礎攻撃力
		int currentPower = 0;				//現在の攻撃力(自身の攻撃力+武器の攻撃力)
		float maxStamina = 0;			//スタミナの最大値
		float staminaRecoveryRate = 0.0f;		//スタミナの回復速度倍率
		float currentStamina = 0;		//現在のスタミナ
		float defaultSpeed = 0.0f;		//基礎スピード
		float dashSpeed = 0.0f;			//加速時のスピード
		float damagedInvisibleTimeLimit = 0.0f;		//被ダメージ後の無敵時間
	};


	PlayerStatus m_playerStatus;

public:

	/// <summary>
	/// キャラクターのステータス構造体を取得
	/// </summary>
	/// <returns></returns>
	const PlayerStatus& GetCharacterStatus()
	{
		return m_playerStatus;
	}
	
	/// <summary>
	/// プレイヤーのステータスを初期化
	/// </summary>
	/// <param name="name">ステータスを初期化したいキャラの名前</param>
	void InitPlayerStatus(const std::string& name);


	//////////////////////////////////////////////////////////////////////////////////////////////
	///プレイヤーのステータスに関する関数
	////////////////////////////////////////////////////////////////////////////////////////////// 
	
	/// <summary>
	/// HPの設定
	/// </summary>
	/// <param name="setHp"></param>
	void SetCurrentHp(const int setHp)
	{
		m_playerStatus.currentHp = setHp;
	}
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentHp() const
	{
		return m_playerStatus.currentHp;
	}
	/// <summary>
	/// 最大HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return m_playerStatus.maxHp;
	}

	/// <summary>
	/// 現在の攻撃力の設定(最初に使うだけ)
	/// </summary>
	/// <param name="setAtk"></param>
	void SetCurrentPower(const int setAtk)
	{
		m_playerStatus.currentPower = setAtk;
	}
	/// <summary>
	/// 現在の攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentPower() const
	{
		return m_playerStatus.currentPower;
	}
	/// <summary>
	/// 基礎攻撃力を増やす
	/// </summary>
	/// <param name="addPower">加算する攻撃力</param>
	void AddPowerToDefaultPower(int addPower)
	{
		m_playerStatus.defaultPower += addPower;
	}

	/// <summary>
	/// 武器切り替え時の攻撃力の設定
	/// </summary>
	/// <param name="weaponPower">武器の攻撃力</param>
	void ChangeWeaponCalcCurrentPower(int weaponPower)
	{
		//現在の攻撃力を計算
		m_playerStatus.currentPower = m_playerStatus.defaultPower + weaponPower;
	}

	/// <summary>
	/// 最大スタミナを取得
	/// </summary>
	/// <returns></returns>
	const float& GetMaxStamina() const
	{
		return m_playerStatus.maxStamina;
	}
	/// <summary>
	/// 現在のスタミナを取得
	/// </summary>
	/// <returns></returns>
	const float& GetCurrentStamina() const
	{
		return m_playerStatus.currentStamina;
	}
	
	/// <summary>
	/// スタミナの回復速度倍率を取得
	/// </summary>
	/// <returns></returns>
	float GetStaminaRecoveryRate()
	{
		return m_playerStatus.staminaRecoveryRate;
	}

	/// <summary>
	/// 基礎スピードの取得
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return m_playerStatus.defaultSpeed;
	}
	/// <summary>
	/// ダッシュスピードの取得
	/// </summary>
	/// <returns></returns>
	const float& GetDashSpeed() const
	{
		return m_playerStatus.dashSpeed;
	}

	/// <summary>
	/// 被ダメージ後の無敵時間を取得
	/// </summary>
	/// <returns></returns>
	const float& GetDamagedInvisibleTimeLimit()
	{
		return m_playerStatus.damagedInvisibleTimeLimit;
	}

	///////////////////////////////////////////////////////////
	///
	///////////////////////////////////////////////////////////

	/// <summary>
	/// スタミナを消費できるか。できるときは消費する。できないときは消費しない
	/// </summary>
	/// <param name="consumeValue">消費する量</param>
	/// <returns>消費できたらtrue、消費できなかったらfalse</returns>
	bool TryConsumeStamina(float consumeValue);

	/// <summary>
	/// スタミナを消費できるかチェックする。スタミナは消費しない
	/// </summary>
	/// <param name="checkValue">調べたい値</param>
	/// <returns>消費できるならtrue</returns>
	bool CheckConsumeStamina(float checkValue);

	/// <summary>
	/// スタミナを回復する
	/// </summary>
	/// <param name="recoveryValue">回復する量</param>
	void RecoveryStamina(float recoveryValue);

	/// <summary>
	/// ダメージを受ける。HPが0以下になったらtrueを返す
	/// </summary>
	/// <param name="damage"></param>
	/// <returns></returns>
	bool TakeDamage(int damage)
	{
		//HPからダメージ量を減らす
		m_playerStatus.currentHp -= damage;

		//HPが0以下なら
		if (m_playerStatus.currentHp <= 0)
		{
			m_playerStatus.currentHp = 0;
			//倒された
			return true;
		}

		//倒されていない
		return false;
	}

	/// <summary>
	/// ヒットポイントを回復する
	/// </summary>
	/// <param name="value">回復量</param>
	void RecoveryHp(int value)
	{
		//HPを回復する
		m_playerStatus.currentHp += value;
		//Hpが最大値以上なら
		if (m_playerStatus.currentHp >= m_playerStatus.maxHp)
		{
			m_playerStatus.currentHp = m_playerStatus.maxHp;
		}
	}


	///////////////////////////////////////////////////////////

		//csvは使わない
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, Status_Player> StatusDataMAP;
	//プレイヤーのステータスの初期化で使う静的な型
	static const std::string playerStatusCsvFilePath;
	static bool IsLoadedPlayerStatusCSVFile;


private:

	/// <summary>
	/// プレイヤーステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいプレイヤーの名前</param>
	void LoadPlayerStatusCSV(const std::string& name);


};

