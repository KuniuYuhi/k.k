#include "stdafx.h"
#include "BraveStateAttack_1.h"
#include "Brave.h"

void BraveStateAttack_1::ManageState()
{
	//前進する攻撃フラグが立っている間は処理
	if (m_brave->GetDashAttackFlag() == true)
	{
		//向いている方向に移動
		m_brave->CalcAttackDirection(m_brave->GetNormalAttackSpeed());
	}

	m_brave->ProcessNormalAttackStateTransition();
	
}

void BraveStateAttack_1::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_attack1, 0.2f);
}


