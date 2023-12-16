#include "stdafx.h"
#include "BraveStateAttack_3.h"
#include "Brave.h"

void BraveStateAttack_3::ManageState()
{
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

void BraveStateAttack_3::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack3 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
