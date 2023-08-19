#pragma once
#include "IHeroState.h"

class HeroStateVictory :public IHeroState
{
public:
	HeroStateVictory(Hero* hero)
		:IHeroState(hero)
	{
	}

	/// <summary>
	/// ヒーローの状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};
