#include "stdafx.h"
#include "BraveStateSprint.h"
#include "Brave.h"

void BraveStateSprint::ManageState()
{
	m_brave->ProcessCommonStateTransition();
}

void BraveStateSprint::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Sprint, 0.3f);
}
