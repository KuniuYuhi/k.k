#include "stdafx.h"
#include "CactusStateAttack_2.h"
#include "Cactus.h"

void CactusStateAttack_2::ManageState()
{
	m_cactus->OnProcessAttack_2StateTransition();
}

void CactusStateAttack_2::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Attack_2);
}
