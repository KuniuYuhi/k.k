#pragma once


/// <summary>
/// ウェーブに関するステータス
/// </summary>
class WaveStatus
{
private:


	struct Status
	{
		int totalWaves = 0;						//トータルウェーブ数
		int breakTimeWaveNumber = 0;			//休憩ウェーブの番号
		float waveTimeLimit = 0.0f;				//1ウェーブの時間
		float shortBreakTime = 0.0f;			//ウェーブとウェーブの間の時間
		int maxMobEnemys = 0;					//モブエネミーの最大数
		int firstCreateMobEnemyAmount = 0;		//最初に生成するモブエネミーの数
		int addCreateMobEnemyAmonut = 0;		//追加で生成するモブエネミーの数
		float createRadius = 0.0f;				//生成範囲(半径)
		float coordinateDistance = 0.0f;		//生成座標同士最低限空ける距離
		float bossAvoidanceRadius = 0.0f;		//ボスの中心から生成しない範囲(半径)
		float playerAvoidanceRadius = 0.0f;		//プレイヤーの中心から生成しない範囲(半径)
		float incantationTimeLimit = 0.0f;		//生成位置決定から生成までの時間
	};

	Status m_status;		//ウェーブに関するステータス

	/// <summary>
	/// ウェーブのステータスcsvファイルの読み込み
	/// </summary>
	void LoadWaveStatusCSV();


public:

	/// <summary>
	/// ウェーブのステータスを取得
	/// </summary>
	/// <returns></returns>
	const Status& GetWaveStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ウェーブのステータスを初期化
	/// </summary>
	void InitWaveStatus();



	const int& GetTotalWaves()
	{
		return m_status.totalWaves;
	}

	const int& GetBreakTimeWaveNumber()
	{
		return m_status.breakTimeWaveNumber;
	}

	const float& GetCurrentWaveTimeLimit()
	{
		return m_status.waveTimeLimit;
	}

	const float& GetShortBreakTime()
	{
		return m_status.shortBreakTime;
	}

	const int& GetMaxMobEnemys()
	{
		return m_status.maxMobEnemys;
	}

	const int& GetAddCreateMobEnemyAmonut()
	{
		return m_status.addCreateMobEnemyAmonut;
	}

	const float& GetCreateRadius()
	{
		return m_status.createRadius;
	}

	const float& GetCoordinateDistance()
	{
		return m_status.coordinateDistance;
	}

	const float& GetBossAvoidanceRadius()
	{
		return m_status.bossAvoidanceRadius;
	}

	const float& GetPlayerAvoidanceRadius()
	{
		return m_status.playerAvoidanceRadius;
	}

	const float& GetIncantationTimeLimit()
	{
		return m_status.incantationTimeLimit;
	}



	//CSVの情報を保存したマップ
	static WaveStatus StatusDataMAP;
	//ウェーブのステータスの初期化で使う静的な型
	static const std::string waveStatusCsvFilePath;
	static bool IsLoadedWaveStatusCSVFile;

};

