#include "stdafx.h"
#include "WizardStateRun.h"
#include "Wizard.h"
//����X�e�[�g�N���X
void WizardStateRun::ManageState()
{
	m_wizard->ProcessCommonStateTransition();
}

void WizardStateRun::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Run);
}
