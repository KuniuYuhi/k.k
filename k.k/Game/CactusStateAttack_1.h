#pragma once
#include "ICactusState.h"

class CactusStateAttack_1 :public ICactusState
{
public:
	CactusStateAttack_1(Cactus* cactus)
		:ICactusState(cactus)
	{
	}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
};
