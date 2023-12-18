#pragma once
#include "ISlimeState.h"

class SlimeStateChase :public ISlimeState
{
public:
	SlimeStateChase(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};