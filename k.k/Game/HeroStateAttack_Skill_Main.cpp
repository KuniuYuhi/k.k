#include "stdafx.h"
#include "HeroStateAttack_Skill_Main.h"
#include "Hero.h"
//スキルメインステート
void HeroStateAttack_Skill_Main::ManageState()
{
	m_hero->OnProcessAttack_Skill_MainStateTransition();
}

void HeroStateAttack_Skill_Main::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_Skill_Main);
}
