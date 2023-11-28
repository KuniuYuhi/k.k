#pragma once
#include "ISummonerState.h"

class SummonerState_DarkBall :public ISummonerState
{
public:
	SummonerState_DarkBall(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Darkball)
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
