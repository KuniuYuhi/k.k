#pragma once
#include "ISummonerState.h"

class SummonerState_Command :public ISummonerState
{
public:
	SummonerState_Command(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Command)
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
