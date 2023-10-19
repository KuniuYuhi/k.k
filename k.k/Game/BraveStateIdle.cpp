#include "stdafx.h"
#include "BraveStateIdle.h"
#include "Brave.h"

void BraveStateIdle::ManageState()
{
	m_brave->ProcessCommonStateTransition();
}

void BraveStateIdle::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Idle, 0.5f);
}
