#pragma once
#include "IBossStateMachine.h"

class SummonerSM_Attack:public IBossStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="summoner"></param>
	SummonerSM_Attack(Summoner* summoner)
		:IBossStateMachine(summoner)
	{
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SummonerSM_Attack(){}

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	virtual void Execute() override;

private:

	

	/// <summary>
	/// ランダムな値を設定
	/// </summary>
	void SetRandomActionValue()
	{
		m_randomValue = rand() % randomMaxValue;
	}

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

	/// <summary>
	/// 遠距離攻撃の中からステートを決める
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// 近距離攻撃の中からステートを決める
	/// </summary>
	void ProcessMeleeAttack();

	/// <summary>
	///	ダークボールのアニメーションに移るための処理
	/// </summary>
	void IsNextActionDarkBall();

	/// <summary>
	/// 次の行動をダークメテオにするか決める
	/// </summary>
	/// <returns>ダークメテオならtrue</returns>
	bool IsNextActionDarkMeteo();

private:
	Vector3 m_toPlayer = g_vec3Zero;

	float m_continuousAttackTimer = 0.0f;		//待機時間を計算するタイマー

	int attackRetryProbability = 3;	//もう一度攻撃する確率
	const int randomMaxValue = 10;	//抽選する時の最大値
	bool m_lotteryChanceFlag = false;		//もう一度攻撃するか抽選するチャンス
	int m_randomValue = 0;

	int m_meleeCount = 0;			//近距離攻撃をする度にカウント


};

