#include "stdafx.h"
#include "LichStateDarkMeteorite_Start.h"
#include "Lich.h"

void LichStateDarkMeteorite_Start::ManageState()
{
	m_lich->OnProcessDarkMeteorite_StartStateTransition();
}

void LichStateDarkMeteorite_Start::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Attack_DarkMeteorite_start);
}
