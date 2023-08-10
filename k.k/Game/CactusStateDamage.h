#pragma once
#include "ICactusState.h"

class CactusStateDamage :public ICactusState
{
public:
	CactusStateDamage(Cactus* cactus)
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
