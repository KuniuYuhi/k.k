#include "stdafx.h"
#include "HeroStateDie.h"
#include "Hero.h"

void HeroStateDie::ManageState()
{
	m_hero->OnProcessDieStateTransition();
}

void HeroStateDie::PlayAnimation()
{
	m_hero->GetModelRender().SetAnimationSpeed(0.7f);
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_Die);
}
