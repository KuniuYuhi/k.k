#pragma once
#include "ISummonerState.h"

class SummonerState_Hit :public ISummonerState
{
public:
	SummonerState_Hit(Summoner* summoner)
		:ISummonerState(summoner, enActionName_CriticalHit)
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

