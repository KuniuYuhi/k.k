#include "stdafx.h"
#include "HeroStateDamage.h"
#include "Hero.h"

void HeroStateDamage::ManageState()
{
	m_hero->OnProcessDamageStateTransition();
}

void HeroStateDamage::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Damage);
}
