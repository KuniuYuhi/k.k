#include "stdafx.h"
#include "WizardStateVictory_start.h"
#include "Wizard.h"

void WizardStateVictory_start::ManageState()
{
	m_wizard->OnProcessVictoryStateTransition();
}

void WizardStateVictory_start::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Victory_start,0.3f);
}
