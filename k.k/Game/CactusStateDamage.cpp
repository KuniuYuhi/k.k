#include "stdafx.h"
#include "CactusStateDamage.h"
#include "Cactus.h"

void CactusStateDamage::ManageState()
{
	m_cactus->OnProcessDamageStateTransition();
}

void CactusStateDamage::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Damage);
}
