#pragma once
#include "ISlimeState.h"

class SlimeStateDamage:public ISlimeState
{
public:
	SlimeStateDamage(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

