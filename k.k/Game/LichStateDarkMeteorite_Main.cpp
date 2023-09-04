#include "stdafx.h"
#include "LichStateDarkMeteorite_Main.h"
#include "Lich.h"

void LichStateDarkMeteorite_Main::ManageState()
{
	m_lich->OnProcessDarkMeteorite_MainStateTransition();
}

void LichStateDarkMeteorite_Main::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Attack_DarkMeteorite_main,0.8f);
}
