#include "stdafx.h"
#include "LichStateDarkMeteorite_End.h"
#include "Lich.h"

void LichStateDarkMeteorite_End::ManageState()
{
	m_lich->OnProcessDarkMeteorite_EndStateTransition();
}

void LichStateDarkMeteorite_End::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Attack_DarkMeteorite_end);
}
