#include "stdafx.h"
#include "BraveStateChangeSwordShield.h"
#include "Brave.h"

void BraveStateChangeSwordShield::ManageState()
{
	m_brave->ProcessCommonWeaponChangeStateTransition();
}

void BraveStateChangeSwordShield::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_ChangeSwordShield + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(), 
		0.2f
	);
}
