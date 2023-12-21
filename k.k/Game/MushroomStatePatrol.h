#pragma once
#include "IMushroomState.h"

class MushroomStatePatrol :public IMushroomState
{
public:
	MushroomStatePatrol(Mushroom* mushroom)
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