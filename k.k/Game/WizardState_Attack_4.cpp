#include "stdafx.h"
#include "WizardState_Attack_4.h"
#include "Wizard.h"

void WizardState_Attack_4::ManageState()
{
	m_wizard->OnProcessAttack_4StateTransition();
}

void WizardState_Attack_4::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_4);
}
