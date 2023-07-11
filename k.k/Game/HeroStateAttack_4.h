#pragma once
#include "IHeroState.h"

class HeroStateAttack_4:public IHeroState
{
public:
	HeroStateAttack_4(Hero* hero)
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

