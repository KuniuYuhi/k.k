#pragma once
#include "ICactusState.h"

class CactusStateIdle :public ICactusState
{
public:
	CactusStateIdle(Cactus* cactus)
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
