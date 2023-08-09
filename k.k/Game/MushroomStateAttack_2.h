#pragma once
#include "IMushroomState.h"

class MushroomStateAttack_2 :public IMushroomState
{
public:
	MushroomStateAttack_2(Mushroom* mushroom)
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
