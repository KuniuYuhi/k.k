#include "stdafx.h"
#include "BraveStateWin_Start.h"
#include "Brave.h"

void BraveStateWin_Start::ManageState()
{
}

void BraveStateWin_Start::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Win_Start + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
