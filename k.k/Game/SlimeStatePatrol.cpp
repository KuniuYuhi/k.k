#include "stdafx.h"
#include "SlimeStatePatrol.h"
#include "Slime.h"

void SlimeStatePatrol::ManageState()
{
	//„‰ñ‚·‚é
	m_slime->MovePatrol(m_slime->GetCharacterController());
}

void SlimeStatePatrol::PlayAnimation()
{
	if (fabsf(m_slime->GetMoveSpeed().x) >= 0.001f || fabsf(m_slime->GetMoveSpeed().z) >= 0.001f)
	{
		m_slime->GetModelRender().PlayAnimation(enAnimationClip_Patrol, 0.2f);
	}
	else
	{
		m_slime->GetModelRender().PlayAnimation(enAnimationClip_Idle, 0.4f);
	}

	//m_slime->GetModelRender().PlayAnimation(enAninationClip_Patrol,0.3f);
}
