#pragma once
#include "IHeroState.h"
//����X�e�[�g�N���X
class HeroStateRun:public IHeroState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HeroStateRun(Hero* hero)
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

