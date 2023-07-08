#include "stdafx.h"
#include "WizardStateIdle.h"
#include "Wizard.h"
//待機ステートクラス
void WizardStateIdle::ManageState()
{
	m_wizard->ProcessCommonStateTransition();
}

void WizardStateIdle::PlayAnimation()
{
	//待機アニメーションの再生
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Idle);
}
