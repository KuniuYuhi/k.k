#include "stdafx.h"
#include "LichStateDamage.h"
#include "Lich.h"

void LichStateDamage::ManageState()
{
	m_lich->OnProcessDamageStateTransition();
}

void LichStateDamage::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Damage);
}
