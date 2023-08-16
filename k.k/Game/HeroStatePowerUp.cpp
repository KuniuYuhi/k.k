#include "stdafx.h"
#include "HeroStatePowerUp.h"
#include "Hero.h"

void HeroStatePowerUp::ManageState()
{
	m_hero->OnProcessPowerUpStateTransition();
}

void HeroStatePowerUp::PlayAnimation()
{
	m_hero->GetModelRender().PlayAnimation(Hero::enAnimClip_PowerUp);
}
