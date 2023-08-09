#pragma once
#include "IMushroomState.h"

class MushroomStateAttack_1 :public IMushroomState
{
public:
	MushroomStateAttack_1(Mushroom* mushroom)
		:IMushroomState(mushroom)
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
