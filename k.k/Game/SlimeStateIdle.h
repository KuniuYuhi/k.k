#pragma once
#include "ISlimeState.h"

class SlimeStateIdle:public ISlimeState
{
public:
	SlimeStateIdle(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

