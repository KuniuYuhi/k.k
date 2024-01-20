#include "stdafx.h"
#include "BraveStateSkill_Main.h"
#include "Brave.h"

void BraveStateSkill_Main::ManageState()
{
	//後退するフラグが立っていたら処理する
	if (m_brave->GetMoveBackFlag() == true)
	{
		//向いている方向に移動
		//武器のスピードにする
		m_brave->MoveBack(m_brave->GetWeapon(enWeapon_Main)->GetMoveBackSpeed());
	}

	m_brave->ProcessSkillMainStateTransition();
}

void BraveStateSkill_Main::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Skill_Main + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
