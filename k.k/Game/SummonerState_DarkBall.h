#pragma once
#include "ISummonerState.h"

class SummonerState_DarkBall :public ISummonerState
{
public:
	SummonerState_DarkBall(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Darkball)
	{
		//Init();
	}

	~SummonerState_DarkBall();

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
	//void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ダークボールの生成
	/// </summary>
	void CreateDarkBall();

private:

	

};
