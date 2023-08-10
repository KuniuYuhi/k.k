#pragma once
#include "ISlimeState.h"

class SlimeStateWalk:public ISlimeState
{
public:
	SlimeStateWalk(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

