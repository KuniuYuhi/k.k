#pragma once
#include "ISummonerState.h"

class SummonerState_DarkWall :public ISummonerState
{
public:
	SummonerState_DarkWall(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkWall)
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