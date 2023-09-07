#pragma once
#include "ISlimeState.h"

class SlimeStateAppear :public ISlimeState
{
public:
	SlimeStateAppear(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

