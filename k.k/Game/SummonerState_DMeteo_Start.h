#pragma once
#include "ISummonerState.h"

class DarkMeteo;

class SummonerState_DMeteo_Start :public ISummonerState
{
public:
	SummonerState_DMeteo_Start(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Start)
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
	DarkMeteo* m_darkMeteo = nullptr;

};
