#include "stdafx.h"
#include "WizardStateWalk.h"
#include "Wizard.h"
//歩きステートクラス
void WizardStateWalk::ManageState()
{
	m_wizard->ProcessCommonStateTransition();
}

void WizardStateWalk::PlayAnimation()
{
	//歩きアニメーションの再生
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Walk,0.3f);
}
