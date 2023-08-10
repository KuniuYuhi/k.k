#pragma once
#include "ISlimeState.h"

class SlimeStateVictory :public ISlimeState
{
public:
	SlimeStateVictory(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

