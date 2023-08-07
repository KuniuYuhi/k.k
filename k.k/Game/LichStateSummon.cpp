#include "stdafx.h"
#include "LichStateSummon.h"
#include "Lich.h"

void LichStateSummon::ManageState()
{
	m_lich->OnProcessSummonStateTransition();
}

void LichStateSummon::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Summon);
}
