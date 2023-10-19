#include "stdafx.h"
#include "BraveStateAttack_3.h"
#include "Brave.h"

void BraveStateAttack_3::ManageState()
{
	m_brave->ProcessNormalAttackStateTransition();
}

void BraveStateAttack_3::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_attack3, 0.2f);
}
