#include "stdafx.h"
#include "WizardStateVictory_main.h"
#include "Wizard.h"

void WizardStateVictory_main::ManageState()
{
	m_wizard->OnProcessVictoryStateTransition();
}

void WizardStateVictory_main::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Victory_main);
}
