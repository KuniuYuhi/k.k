#pragma once
#include "ISlimeState.h"

class SlimeStateSkill :public ISlimeState
{
public:
	SlimeStateSkill(Slime* slime)
		:ISlimeState(slime)
	{
	}

	void ManageState();

	void PlayAnimation();
};
