#include "stdafx.h"
#include "SlimeStateDie.h"
#include "Slime.h"

void SlimeStateDie::ManageState()
{
	m_slime->OnProcessDieStateTransition();
}

void SlimeStateDie::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Die);
}
