#include "stdafx.h"
#include "HeroStateAttack_4.h"
#include "Hero.h"
//�A�^�b�N�S�X�e�[�g
void HeroStateAttack_4::ManageState()
{
	m_hero->OnProcessAttack_4StateTransition();
}

void HeroStateAttack_4::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_4);
}
