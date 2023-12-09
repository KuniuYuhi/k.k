#pragma once

#include "ISummonerState.h"

class SummonerState_DMeteo_Main :public ISummonerState
{
public:
	SummonerState_DMeteo_Main(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Main)
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