#include "stdafx.h"
#include "WizardStateAttack_1.h"
#include "Wizard.h"

void WizardStateAttack_1::ManageState()
{
	m_wizard->OnProcessAttack_1StateTransition();
}

void WizardStateAttack_1::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_1);
}
