#pragma once
#include "IHeroState.h"
//歩きステートクラス
class HeroStateWalk:public IHeroState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HeroStateWalk(Hero* hero)
		//ヒーローのインスタンスを代入する
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

