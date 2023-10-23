#include "stdafx.h"
#include "BraveStateAttack_1.h"
#include "Brave.h"

void BraveStateAttack_1::ManageState()
{
	//前進するフラグが立っている間は処理
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//向いている方向に移動
		m_brave->MoveForward(m_brave->GetNormalAttackSpeed());
	}

	m_brave->ProcessNormalAttackStateTransition();
	
}

void BraveStateAttack_1::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack1 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}


