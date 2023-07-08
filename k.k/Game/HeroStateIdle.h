#pragma once
#include "IHeroState.h"
//待機ステートクラス
class HeroStateIdle : public IHeroState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HeroStateIdle(Hero* hero) 
		//ヒーローのインスタンスを代入する
		: IHeroState(hero) 
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

