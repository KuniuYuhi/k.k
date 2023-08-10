#include "stdafx.h"
#include "LichStateVictory.h"
#include "Lich.h"

void LichStateVictory::ManageState()
{
	m_lich->OnProcessVictoryStateTransition();
}

void LichStateVictory::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Victory);
}
