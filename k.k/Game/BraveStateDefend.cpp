#include "stdafx.h"
#include "BraveStateDefend.h"
#include "Brave.h"

void BraveStateDefend::ManageState()
{
	//Yボタンを離したら防御状態解除
	if (g_pad[0]->IsPress(enButtonY) != true)
	{
		//防御状態が解除されたのでアクションフラグをfalseにする
		m_brave->SetIsActionFlag(false);
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveStateDefend::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Defend + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);

}
