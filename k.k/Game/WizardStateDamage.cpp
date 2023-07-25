#include "stdafx.h"
#include "WizardStateDamage.h"
#include "Wizard.h"

void WizardStateDamage::ManageState()
{
	m_wizard->OnProcessDamageStateTransition();
}

void WizardStateDamage::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Damage);
}
