#include "stdafx.h"
#include "HeroStateAttack_1.h"
#include "Hero.h"
//アタック１ステートクラス
void HeroStateAttack_1::ManageState()
{
	m_hero->OnProcessAttack_1StateTransition();
}

void HeroStateAttack_1::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Attack_1);
}
