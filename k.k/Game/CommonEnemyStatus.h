#pragma once


/// <summary>
/// エネミーに共通するステータス
/// </summary>
class CommonEnemyStatus
{
private:

	/// <summary>
	/// エネミーで共通するステータス
	/// </summary>
	struct EnemyStatus
	{
		int maxHp = 0;				//最大HP
		int currentHp = 0;			//現在のHP
		int defaultPower = 0;		//基礎攻撃力
		int currentPower = 0;		//現在の攻撃力
		float defaultSpeed = 0;		//基礎スピード
		float attackIntarval = 0;	//攻撃インターバル

		float approachDistance = 0.0f;	//接近できる最大の距離
		float waitingDistance = 0.0f;	//待機する距離

	};


	EnemyStatus m_commonEnemyStatus;		//エネミー共通のステータス

public:

	/// <summary>
	/// エネミーの共通ステータスを初期化
	/// </summary>
	/// <param name="name">ステータスを初期化したいキャラクターの名前</param>
	void InitCommonEnemyStatus(const std::string& name);


	/// <summary>
	/// エネミーの共通ステータスを取得
	/// </summary>
	/// <returns></returns>
	const EnemyStatus& GetCommonEnemyStatus()
	{
		return m_commonEnemyStatus;
	}


	/// <summary>
	/// HPの設定
	/// </summary>
	/// <param name="setHp"></param>
	void SetCurrentHp(const int setHp)
	{
		m_commonEnemyStatus.currentHp = setHp;
	}
	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentHp() const
	{
		return m_commonEnemyStatus.currentHp;
	}
	/// <summary>
	/// 最大HPの取得
	/// </summary>
	/// <returns></returns>
	const int& GetMaxHp() const
	{
		return m_commonEnemyStatus.maxHp;
	}

	/// <summary>
	/// 現在の攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetCurrentAtk(const int setAtk)
	{
		m_commonEnemyStatus.currentPower = setAtk;
	}
	/// <summary>
	/// 現在の攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentAtk() const
	{
		return m_commonEnemyStatus.currentPower;
	}

	/// <summary>
	/// スピードを取得
	/// </summary>
	/// <returns></returns>
	const float& GetDefaultSpeed() const
	{
		return m_commonEnemyStatus.defaultSpeed;
	}

	/// <summary>
	/// 接近できる距離を取得
	/// </summary>
	/// <returns></returns>
	const float& GetApproachDistance() const
	{
		return m_commonEnemyStatus.approachDistance;
	}
	/// <summary>
	/// 待機する距離を取得
	/// </summary>
	/// <returns></returns>
	const float& GetWaitingDistance() const
	{
		return m_commonEnemyStatus.waitingDistance;
	}


	//csvは使わない
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, CommonEnemyStatus> StatusDataMAP;
	//エネミーのステータスの初期化で使う静的な型
	static const std::string commonEnemyStatusCsvFilePath;
	static bool IsLoadedCommonEnemyStatusCSVFile;

private:

	/// <summary>
	/// エネミーの共通ステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいエネミーの名前</param>
	void LoadCommonEnemyStatusCSV(const std::string& name);

};

