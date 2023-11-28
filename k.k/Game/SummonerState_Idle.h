#pragma once
#include "ISummonerState.h"

namespace {
	const int PRIORITY = 1;
}

class SummonerState_Idle:public ISummonerState
{
public:
	SummonerState_Idle(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Idle)
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

