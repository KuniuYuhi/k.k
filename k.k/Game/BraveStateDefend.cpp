#include "stdafx.h"
#include "BraveStateDefend.h"
#include "Brave.h"

namespace {
	const int DEFENCE = 0;
	const int AVOID = 1;
}

void BraveStateDefend::ManageState()
{
	//武器によって違う処理
	switch (m_brave->GetMainWeaponDefendTipe())
	{
	case DEFENCE:
		ProcessDefence();
		break;
	case AVOID:
		ProcessAvoid();
		break;
	default:
		break;
	}
}

void BraveStateDefend::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Defend + m_brave->GetCurrentMainWeaponAnimationStartIndexNo()
	);

}

void BraveStateDefend::ProcessDefence()
{
	//Yボタンを離したら防御状態解除
	if (g_pad[0]->IsPress(enButtonY) != true)
	{
		//防御状態が解除されたのでアクションフラグをfalseにする
		m_brave->SetAllInfoAboutActionFlag(false);
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveStateDefend::ProcessAvoid()
{
	//前進するフラグが立っている間は処理
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//向いている方向に移動
		m_brave->MoveForward(m_brave->GetAvoidSpeed());
	}

	m_brave->ProcessDefendStateTransition();
}
