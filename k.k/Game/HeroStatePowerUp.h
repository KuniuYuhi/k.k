#pragma once
#include "IHeroState.h"

class HeroStatePowerUp:public IHeroState
{
public:
	HeroStatePowerUp(Hero* hero)
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

