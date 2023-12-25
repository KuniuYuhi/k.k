#pragma once
#include "ISummonerState.h"

class SummonerState_DarkSpearStart :public ISummonerState
{
public:
	SummonerState_DarkSpearStart(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_Start)
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
