#include "stdafx.h"
#include "SlimeStateAttack.h"
#include "Slime.h"

void SlimeStateAttack::ManageState()
{
	m_slime->OnProcessAttack_1StateTransition();
}

void SlimeStateAttack::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(Slime::enAnimClip_Attack_1);
}
