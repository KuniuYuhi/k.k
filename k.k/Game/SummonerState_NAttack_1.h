#pragma once
#include "ISummonerState.h"

class SummonerState_NAttack_1 :public ISummonerState
{
public:
	SummonerState_NAttack_1(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Darkball)
	{
	}
	/// <summary>
	/// 状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();


private:

	/// <summary>
	/// プレイヤーが範囲内にいるか
	/// </summary>
	/// <returns></returns>
	bool IsPlayerInRange();

	/// <summary>
	/// 次の行動を決める
	/// </summary>
	void NextAction();

private:

	int m_probability = 10;	//攻撃か待機かの確率

};
