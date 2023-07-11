#include "stdafx.h"
#include "WizardStateAttack_3_Main.h"
#include "Wizard.h"

void WizardStateAttack_3_Main::ManageState()
{
	m_wizard->OnProcessAttack_3StateTransition();
}

void WizardStateAttack_3_Main::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Attack_3_main);
}
