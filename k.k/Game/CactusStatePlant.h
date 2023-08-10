#pragma once
#include "ICactusState.h"

class CactusStatePlant :public ICactusState
{
public:
	CactusStatePlant(Cactus* cactus)
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
