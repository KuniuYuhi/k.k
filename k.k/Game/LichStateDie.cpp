#include "stdafx.h"
#include "LichStateDie.h"
#include "Lich.h"

void LichStateDie::ManageState()
{
	m_lich->OnProcessDieStateTransition();
}

void LichStateDie::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Die);
}
