#pragma once
#include "ISummonerState.h"

class SummonerState_Warp :public ISummonerState
{
public:
	SummonerState_Warp(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Warp)
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
