#pragma once
#include "ISlimeState.h"

class SlimeStateAttack :public ISlimeState
{
public:
	SlimeStateAttack(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};

