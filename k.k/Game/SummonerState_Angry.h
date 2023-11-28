#pragma once
#include "ISummonerState.h"
class SummonerState_Angry : public ISummonerState
{
public:
	SummonerState_Angry(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Angry)
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
	/// 怒りモードである時間の計算
	/// </summary>
	/// <returns>trueで怒りモード終了</returns>
	bool CalcAngryModeTimer();

private:

	const float m_angryModeLimmitTime = 10.0f;	//怒りモードでいられる秒数
	float m_angryModeTimer = m_angryModeLimmitTime;

	bool m_angryModeFlag = false;				//怒りモードであるかのフラグ

};

