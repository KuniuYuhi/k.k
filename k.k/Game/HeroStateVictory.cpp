#include "stdafx.h"
#include "HeroStateVictory.h"
#include "Hero.h"

void HeroStateVictory::ManageState()
{
	m_hero->OnProcessVictoryStateTransition();
}

void HeroStateVictory::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Victory);
}
