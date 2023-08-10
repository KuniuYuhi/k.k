#include "stdafx.h"
#include "SlimeStateIdle.h"
#include "Slime.h"

void SlimeStateIdle::ManageState()
{
	m_slime->ProcessCommonStateTransition();
}

void SlimeStateIdle::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(Slime::enAnimClip_Idle);
}
