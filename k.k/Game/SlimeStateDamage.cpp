#include "stdafx.h"
#include "SlimeStateDamage.h"
#include "Slime.h"

void SlimeStateDamage::ManageState()
{
	m_slime->OnProcessDamageStateTransition();
}

void SlimeStateDamage::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(Slime::enAnimClip_Damage);
}
