#include "stdafx.h"
#include "WizardStateDie.h"
#include"Wizard.h"

void WizardStateDie::ManageState()
{
	m_wizard->OnProcessDieStateTransition();
}

void WizardStateDie::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Die);
}
