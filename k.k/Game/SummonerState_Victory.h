#pragma once
#include "ISummonerState.h"

class SummonerState_Victory :public ISummonerState
{
public:
	SummonerState_Victory(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Victory)
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
