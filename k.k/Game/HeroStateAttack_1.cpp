#include "stdafx.h"
#include "HeroStateAttack_1.h"
#include "Hero.h"
//�A�^�b�N�P�X�e�[�g�N���X
void HeroStateAttack_1::ManageState()
{
	m_hero->OnProcessAttack_1StateTransition();
}

void HeroStateAttack_1::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_1);
}
