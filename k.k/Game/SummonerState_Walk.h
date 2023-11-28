#pragma once
#include "ISummonerState.h"

class SummonerState_Walk :public ISummonerState
{
public:
	SummonerState_Walk(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Walk)
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