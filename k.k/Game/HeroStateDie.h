#pragma once
#include "IHeroState.h"

class HeroStateDie:public IHeroState
{
public:
	HeroStateDie(Hero* hero)
		:IHeroState(hero)
	{
	}

	/// <summary>
	/// �q�[���[�̏�ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
};

