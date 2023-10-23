#pragma once
#include "IBraveState.h"

class BraveStateDefend :public IBraveState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="brave"></param>
	BraveStateDefend(Brave* brave)
		:IBraveState(brave)
	{
	}

	/// <summary>
	/// 勇者の状態遷移管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();

private:
	/// <summary>
	/// 盾などで防ぐときの処理
	/// </summary>
	void ProcessDefence();
	/// <summary>
	/// 回避するときの処理
	/// </summary>
	void ProcessAvoid();
};

