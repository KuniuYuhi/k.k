#pragma once

#include "ISummonerState.h"

class DarkMeteo;

class SummonerState_DMeteo_Main :public ISummonerState
{
public:
	SummonerState_DMeteo_Main(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Main)
	{
		Init();
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

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
	/// アニメーションイベント
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	void OnDarkMeteoAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void ProcessShotMeteo();

private:
	DarkMeteo* m_darkMeteo = nullptr;

};