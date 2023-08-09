#pragma once
#include "IMushroomState.h"

class MushroomStateDamage :public IMushroomState
{
public:
	MushroomStateDamage(Mushroom* mushroom)
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
