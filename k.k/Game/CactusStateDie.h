#pragma once
#include "ICactusState.h"

class CactusStateDie :public ICactusState
{
public:
	CactusStateDie(Cactus* cactus)
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
