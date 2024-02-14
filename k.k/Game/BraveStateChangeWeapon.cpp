#include "stdafx.h"
#include "BraveStateChangeWeapon.h"
#include "Brave.h"

void BraveStateChangeWeapon::ManageState()
{
	m_brave->ProcessCommonWeaponChangeStateTransition();
}

void BraveStateChangeWeapon::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_ChangeSwordShield + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(), 
		0.2f
	);
}
