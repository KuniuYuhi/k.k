#include "stdafx.h"
#include "HeroStateAttack_4.h"
#include "Hero.h"
//アタック４ステート
void HeroStateAttack_4::ManageState()
{
	m_hero->OnProcessAttack_Skill_ChargeStateTransition();
}

void HeroStateAttack_4::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_Skill_Charge);
}
