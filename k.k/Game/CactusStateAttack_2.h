#pragma once
#include "ICactusState.h"

class CactusStateAttack_2 :public ICactusState
{
public:
	CactusStateAttack_2(Cactus* cactus)
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
