#include "stdafx.h"
#include "BraveStateDie.h"
#include "Brave.h"

void BraveStateDie::ManageState()
{
	m_brave->ProcessDieStateTransition();
}

void BraveStateDie::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Die, 0.2f);
}
