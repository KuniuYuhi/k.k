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

	/// <summary>
	/// 死亡時のエフェクト再生
	/// </summary>
	void CreateDeadEffect();


private:

	EffectEmitter* m_deadEffect = nullptr;	//死亡エフェクト
	bool m_isdeadEffectPlayFlag = false;
};
