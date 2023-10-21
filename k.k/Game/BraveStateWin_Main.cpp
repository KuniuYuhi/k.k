#include "stdafx.h"
#include "BraveStateWin_Main.h"
#include "Brave.h"

void BraveStateWin_Main::ManageState()
{
}

void BraveStateWin_Main::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Win_Main + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(), 
		0.2f
	);
}
