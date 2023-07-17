#include "stdafx.h"
#include "LichStateAttack_2.h"
#include "Lich.h"

void LichStateAttack_2::ManageState()
{
	m_lich->OnProcessAttack_2StateTransition();
}

void LichStateAttack_2::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Attack_2);
}
