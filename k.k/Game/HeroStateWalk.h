#pragma once
#include "IHeroState.h"
//�����X�e�[�g�N���X
class HeroStateWalk:public IHeroState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HeroStateWalk(Hero* hero)
		//�q�[���[�̃C���X�^���X��������
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

