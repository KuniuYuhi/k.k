#include "stdafx.h"
#include "HeroStateAttack_2.h"
#include "Hero.h"
//アタック２ステート
void HeroStateAttack_2::ManageState()
{
	m_hero->OnProcessAttack_2StateTransition();
}

void HeroStateAttack_2::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_2);
}
