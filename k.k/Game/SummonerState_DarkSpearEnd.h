#pragma once
#include "ISummonerState.h"

class SummonerState_DarkSpearEnd :public ISummonerState
{
public:
	SummonerState_DarkSpearEnd(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_End)
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
