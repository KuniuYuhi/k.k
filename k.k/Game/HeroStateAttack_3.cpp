#include "stdafx.h"
#include "HeroStateAttack_3.h"
#include "Hero.h"

//�A�^�b�N�P�X�e�[�g�N���X
void HeroStateAttack_3::ManageState()
{
	m_hero->OnProcessAttack_3StateTransition();
}

void HeroStateAttack_3::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_3);
}
