#pragma once
#include "IHeroState.h"
//�ҋ@�X�e�[�g�N���X
class HeroStateIdle : public IHeroState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HeroStateIdle(Hero* hero) 
		//�q�[���[�̃C���X�^���X��������
		: IHeroState(hero) 
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

