#include "stdafx.h"
#include "LichStateAttack_1.h"
#include "Lich.h"

void LichStateAttack_1::ManageState()
{
	m_lich->OnProcessAttack_1StateTransition();
}

void LichStateAttack_1::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Attack_1);
}
