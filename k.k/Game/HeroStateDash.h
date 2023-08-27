#pragma once
#include "IHeroState.h"
//走るステートクラス
class HeroStateDash:public IHeroState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HeroStateDash(Hero* hero)
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

