#pragma once

#include "ISummonerState.h"

class DarkSpear;

class SummonerState_DarkSpearMain :public ISummonerState
{
public:
	SummonerState_DarkSpearMain(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_Main)
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
	/// <summary>
	/// ダークスピアを生成
	/// </summary>
	void CreateDarkSpear();

private:

	DarkSpear* m_darkSpear = nullptr;

	bool m_createDarkSpearFag = false;
};
