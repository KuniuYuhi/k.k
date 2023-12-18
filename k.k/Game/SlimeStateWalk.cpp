#include "stdafx.h"
#include "SlimeStateWalk.h"
#include "Slime.h"

void SlimeStateWalk::ManageState()
{
	m_slime->ProcessCommonStateTransition();
}

void SlimeStateWalk::PlayAnimation()
{
	//m_slime->GetModelRender().PlayAnimation(Slime::enAnimClip_Walk);
}
