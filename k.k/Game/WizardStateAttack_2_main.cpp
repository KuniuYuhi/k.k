#include "stdafx.h"
#include "WizardStateAttack_2_main.h"
#include "Wizard.h"

void WizardStateAttack_2_main::ManageState()
{
	m_wizard->OnProcessAttack_2MainStateTransition();
}

void WizardStateAttack_2_main::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_2_main);
}
