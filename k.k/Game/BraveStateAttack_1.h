#pragma once
#include "IBraveState.h"

class BraveStateAttack_1 :public IBraveState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="brave"></param>
	BraveStateAttack_1(Brave* brave)
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
};
