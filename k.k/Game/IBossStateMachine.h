#pragma once
#include "SummonerActionList.h"

class Summoner;
class ISummonerState;

class IBossStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bossInstance"></param>
	IBossStateMachine(Summoner* bossInstance);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~IBossStateMachine();

	/// <summary>
	/// インスタンスを生成
	/// </summary>
	/// <param name="bossInstance"></param>
	static void CreateInstance(Summoner* bossInstance)
	{
		m_stateMachineInstance = new IBossStateMachine(bossInstance);
	}
	/// <summary>
	/// インスタンスの削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_stateMachineInstance;
		m_stateMachineInstance = nullptr;
	}
	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static IBossStateMachine* GetInstance()
	{
		return m_stateMachineInstance;
	}

	/// <summary>
	/// ランダムな値を設定
	/// </summary>
	void SetRandomActionValue()
	{
		m_randomValue = rand() % randomMaxValue;
	}



	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	void Execute();

	/// <summary>
	/// アクションを決める処理
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	/// 次のアクションを決める
	/// </summary>
	void DecideNextAction();

	/// <summary>
	/// ボスがアクション中か
	/// </summary>
	/// <returns></returns>
	bool IsInActionBoss()
	{
		return false;
	}

private:
	/// <summary>
	/// 攻撃ステートを決める
	/// </summary>
	void ProcessAttackState();
	/// <summary>
	///	待機ステートを決める
	/// </summary>
	void ProcessWaitState();

	/// <summary>
	/// 遠距離攻撃の中からステートを決める
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// 近距離攻撃の中からステートを決める
	/// </summary>
	void ProcessMeleeAttack();



	/// <summary>
	/// プレイヤーが近くにとどまっているタイマーを加算する
	/// </summary>
	void AddStayPlayerTimer();

private:
	static IBossStateMachine* m_stateMachineInstance;

	EnActionName m_oldActionName;


	Summoner* m_summoner = nullptr;

	Vector3 m_toPlayer = g_vec3Zero;

	float m_waitTimer = 0.0f;		//待機時間を計算するタイマー

	int attackRetryProbability = 3;	//もう一度攻撃する確率
	const int randomMaxValue = 10;	//抽選する時の最大値
	bool m_lotteryChanceFlag = false;		//もう一度攻撃するか抽選するチャンス
	int m_randomValue = 0;



	int m_meleeCount = 0;			//近距離攻撃をする度にカウント
	float m_stayPlayerTimer = 0.0f;	//プレイヤーが近くにいる間を計るタイマー
	float m_accelerateStayPlayerTimer = 1.0f;	//プレイヤーが近くにいる間を計るタイマーを加速させる
	
};

