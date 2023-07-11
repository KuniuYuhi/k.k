#include "stdafx.h"
#include "WizardStateAttack_3_Start.h"
#include "Wizard.h"

void WizardStateAttack_3_Start::ManageState()
{
	m_wizard->OnProcessAttack_3StateTransition();
}

void WizardStateAttack_3_Start::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_3_start);
}
