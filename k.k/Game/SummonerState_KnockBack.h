#pragma once
#include "ISummonerState.h"

class SummonerState_KnockBack :public ISummonerState
{
public:
	SummonerState_KnockBack(Summoner* summoner)
		:ISummonerState(summoner, enActionName_KnockBack)
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

