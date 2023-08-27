#include "stdafx.h"
#include "WIzardStateWarp.h"
#include "Wizard.h"

void WIzardStateWarp::ManageState()
{
	m_wizard->OnProcessWarpStateTransition();
}

void WIzardStateWarp::PlayAnimation()
{
	m_wizard->GetModelRender().PlayAnimation(Wizard::enAnimClip_Victory_main);
}
