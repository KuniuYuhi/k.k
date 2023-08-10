#include "stdafx.h"
#include "CactusStateVictory.h"
#include "Cactus.h"

void CactusStateVictory::ManageState()
{
	m_cactus->OnProcessVictoryStateTransition();
}

void CactusStateVictory::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Victory);
}
