#include "stdafx.h"
#include "SlimeStatePatrol.h"
#include "Slime.h"

void SlimeStatePatrol::ManageState()
{
	//„‰ñ‚·‚é
	m_slime->MovePatrol(m_slime->GetCharacterController());
	//m_slime->ProcessCommonStateTransition();
	
}

void SlimeStatePatrol::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAninationClip_Patrol,0.3f);
}
