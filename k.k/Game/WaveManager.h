#pragma once
#include "WaveStatus.h"


class WaveStatus;
class EmnemyBase;
class Summoner;

/// <summary>
/// ウェーブ状況を管理するクラス
/// </summary>
class WaveManager:public IGameObject
{
public:

	//魔法陣
	//エネミー生成
	//ウェーブ進行
	//終了時の評価
	//ブレイクタイム


	enum EnWaveManageState
	{
		enWaveState_FirstSummonMobEnemys,		//最初のエネミーの生成
		enWaveState_ProgressWaveTime,			//ウェーブ時間の進行。ループするときはここから
		enWaveState_EvaluateWaveResults,		//ウェーブの評価
		enWaveState_ShortBreakTime,				//小休憩
		enWaveState_SettingCreatePos,			//生成位置のセッティング
		enWaveState_SummonMobEnemys,			//モブエネミー召喚
		enWaveState_BreakTimeWave,				//休憩ウェーブ
	};

	EnWaveManageState m_currentWaveManageState = enWaveState_FirstSummonMobEnemys;
	EnWaveManageState m_nextWaveManageState = enWaveState_FirstSummonMobEnemys;

	//
	// 一定間隔ごとに敵を生成する。
	// 敵はウェーブ切り替え時に削除しない
	// 最大数を超えないように生成
	//

	WaveManager();
	~WaveManager();


	bool Start() override;

	void Update() override;


	const int& GetMobEnemySize()
	{
		return m_mobEnemyList.size();
	}

	const float& GetCurrentWaveTimer()
	{
		return m_currentWaveTimer;
	}

	const float& GetCurrentTimeLimit()
	{
		return m_currentTimeLimit;
	}
	


private:


	////////////////////////////////////////////////////////////////////////
	///ステート関数
	////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// 最初のエネミーの生成
	/// </summary>
	void ProcessFirstSummonMobEnemysStateTransition();
	/// <summary>
	/// ウェーブ時間の進行処理
	/// </summary>
	void ProcessProgressWaveTimeStateTransition();
	/// <summary>
	/// ウェーブの評価
	/// </summary>
	void ProcessEvaluateWaveResultsStateTransition();
	/// <summary>
	/// ウェーブ後の小休憩
	/// </summary>
	void ProcessShortBreakTimeStateTransition();
	/// <summary>
	/// 生成位置のセッティング
	/// </summary>
	void ProcessSettingCreatePosStateTransition();
	/// <summary>
	/// モブエネミー召喚
	/// </summary>
	void ProcessSummonMobEnemysStateTransition();
	/// <summary>
	/// 休憩ウェーブ
	/// </summary>
	void ProcessBreakTimeWaveStateTransition();

	////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// ウェーブタイマーの計算
	/// </summary>
	/// <param name="nextWaveState">次のステート</param>
	void CalcWaveTimer(EnWaveManageState nextWaveState);

	/// <summary>
	/// ウェーブ終了時の評価
	/// </summary>
	void EvaluateWaveResults();


	/// <summary>
	/// ウェーブの進行を管理
	/// </summary>
	void ManageWaveProgression();


	/// <summary>
	/// モブエネミーを召喚する
	/// </summary>
	void SummonMobEnemys();

	/// <summary>
	/// 召喚魔法陣の生成
	/// </summary>
	/// <param name="castAmount"></param>
	void CastSummmonCircle(int castAmount);

	/// <summary>
	/// 召喚するモンスターの座標の設定
	/// </summary>
	/// <param name="radius">生成する範囲の円の半径</param>
	void SetSummonRandomPosition(const int radius,int amount);

	/// <summary>
	/// 他の座標と距離が空いているか
	/// </summary>
	/// <param name="count">調べたい配列の番号</param>
	/// <param name="distance">他の座標との間隔</param>
	/// <returns></returns>
	bool IsWithInDistances(int count, float distance);


	bool IsChangeSummonState();

private:

	Summoner* m_summoner = nullptr;

	WaveStatus m_waveStatus;		//ウェーブのステータス

	std::vector<EmnemyBase*> m_mobEnemyList = {nullptr};		//モブエネミーリスト

	std::vector<Vector3> m_createPositions;

	float m_currentWaveTimer = 0.0f;
	float m_currentTimeLimit = 0.0f;

	int m_currentWaveNumber = 0;


	float m_changeSummonTimer = 0.0f;

	bool m_firstSummonSircleSet = false;

};

