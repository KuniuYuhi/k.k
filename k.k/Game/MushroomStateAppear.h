#pragma once
#include "IMushroomState.h"

class MashroomStateAppear :public IMushroomState
{
public:
	MashroomStateAppear(Mushroom* mushroom)
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
