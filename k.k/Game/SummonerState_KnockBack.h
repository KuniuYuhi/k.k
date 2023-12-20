#pragma once
#include "ISummonerState.h"

class SummonerState_KnockBack :public ISummonerState
{
public:
	SummonerState_KnockBack(Summoner* summoner)
		:ISummonerState(summoner, enActionName_KnockBack)
	{
	}

	~SummonerState_KnockBack();

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
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ノックバック処理
	/// </summary>
	void ProcessKnockBack();

};

