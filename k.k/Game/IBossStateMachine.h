#pragma once
#include "SummonerActionList.h"

class Summoner;
class ISummonerState;

using namespace SummonerActions;

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
	virtual ~IBossStateMachine();

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	virtual void Execute(){}


	/// <summary>
	/// 連続攻撃回数の設定
	/// </summary>
	/// <param name="count"></param>
	void SetContinuousAttackCount(int count)
	{
		m_continuousAttackCount = count;
	}

	/// <summary>
	/// 連続攻撃回数の取得
	/// </summary>
	/// <returns></returns>
	const int& GetContinuousAttackCount() const
	{
		return m_continuousAttackCount;
	}


	/// <summary>
	/// ステイプレイヤータイマーを設定
	/// </summary>
	/// <param name="timer"></param>
	void SetSaveStayPlayerTimer(int timer)
	{
		m_stayPlayerTimer = timer;
	}



protected:
	



protected:

	Summoner* m_summoner = nullptr;

	int m_continuousAttackCount = 0;	//連続攻撃カウント

	static float m_stayPlayerTimer;     //プレイヤーが近くにいる間を計るタイマー



};

