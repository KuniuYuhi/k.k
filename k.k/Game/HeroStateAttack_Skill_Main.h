#pragma once
#include "IHeroState.h"

class HeroStateAttack_Skill_Main:public IHeroState
{
public:
	HeroStateAttack_Skill_Main(Hero* hero)
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

