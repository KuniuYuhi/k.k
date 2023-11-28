#pragma once
#include "ISummonerState.h"

class SummonerState_Die :public ISummonerState
{
public:
	SummonerState_Die(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Die)
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
