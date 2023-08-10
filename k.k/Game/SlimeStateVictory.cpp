#include "stdafx.h"
#include "SlimeStateVictory.h"
#include "Slime.h"

void SlimeStateVictory::ManageState()
{
	m_slime->OnProcessVictoryStateTransition();
}

void SlimeStateVictory::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(Slime::enAnimClip_Victory);
}
