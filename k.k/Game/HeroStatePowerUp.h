#pragma once
#include "IHeroState.h"

class HeroStatePowerUp:public IHeroState
{
public:
	HeroStatePowerUp(Hero* hero)
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

