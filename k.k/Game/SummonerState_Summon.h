#pragma once
#include "ISummonerState.h"

class SummonerState_Summon :public ISummonerState
{
public:
	SummonerState_Summon(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Summon)
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
