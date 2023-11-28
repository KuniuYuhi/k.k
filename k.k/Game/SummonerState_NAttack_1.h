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

};
