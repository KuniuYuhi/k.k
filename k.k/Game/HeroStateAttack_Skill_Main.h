#pragma once
#include "IHeroState.h"

class HeroStateAttack_Skill_Main:public IHeroState
{
public:
	HeroStateAttack_Skill_Main(Hero* hero)
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

