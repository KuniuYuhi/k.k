#pragma once
#include "ISlimeState.h"

class SlimeStateDie :public ISlimeState
{
public:
	SlimeStateDie(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

