#include "stdafx.h"
#include "CactusStateAttack_1.h"
#include "Cactus.h"

void CactusStateAttack_1::ManageState()
{
	m_cactus->OnProcessAttack_1StateTransition();
}

void CactusStateAttack_1::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Attack_1);
}
