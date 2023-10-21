#include "stdafx.h"
#include "BraveStateAttack_2.h"
#include "Brave.h"

void BraveStateAttack_2::ManageState()
{
	m_brave->ProcessNormalAttackStateTransition();
}

void BraveStateAttack_2::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack2 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
