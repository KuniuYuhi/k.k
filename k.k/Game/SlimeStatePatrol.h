#pragma once
#include "ISlimeState.h"

class SlimeStatePatrol :public ISlimeState
{
public:
	SlimeStatePatrol(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};
