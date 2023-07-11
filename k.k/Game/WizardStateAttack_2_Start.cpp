#include "stdafx.h"
#include "WizardStateAttack_2_Start.h"
#include "Wizard.h"

void WizardStateAttack_2_Start::ManageState()
{
	m_wizard->OnProcessAttack_2StateTransition();
}

void WizardStateAttack_2_Start::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_2_start);
}
