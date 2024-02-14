#include "stdafx.h"
#include "BraveStateAttack_2.h"
#include "Brave.h"

void BraveStateAttack_2::ManageState()
{
	//勝敗が着いたらこれ以上動かないようにする
	if (m_brave->IsDecisionOutcomeForState() == true)
	{
		return;
	}

	//前進するフラグが立っている間は処理
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//向いている方向に移動
		//武器のスピードにする
		m_brave->MoveForward(m_brave->GetWeapon(enWeapon_Main)->GetMoveForwardSpeed());
	}
	//後退するフラグが立っていたら処理する
	else if (m_brave->GetMoveBackFlag() == true)
	{
		//向いている方向に移動
		//武器のスピードにする
		m_brave->MoveBack(m_brave->GetWeapon(enWeapon_Main)->GetMoveBackSpeed());
	}

	m_brave->ProcessNormalAttackStateTransition();
}

void BraveStateAttack_2::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack2 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
