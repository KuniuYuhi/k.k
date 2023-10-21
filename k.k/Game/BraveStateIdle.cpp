#include "stdafx.h"
#include "BraveStateIdle.h"
#include "Brave.h"

void BraveStateIdle::ManageState()
{
	m_brave->ProcessCommonStateTransition();
}

void BraveStateIdle::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Idle + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.4f
	);
}
