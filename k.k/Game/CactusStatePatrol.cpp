#include "stdafx.h"
#include "CactusStatePatrol.h"
#include "Cactus.h"

void CactusStatePatrol::ManageState()
{
	//„‰ñ‚·‚é
	m_cactus->MovePatrol(m_cactus->GetCharacterController());
}

void CactusStatePatrol::PlayAnimation()
{
	if (fabsf(m_cactus->GetMoveSpeed().x) >= 0.001f || fabsf(m_cactus->GetMoveSpeed().z) >= 0.001f)
	{
		m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Patrol, 0.2f);
	}
	else
	{
		m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Idle, 0.4f);
	}

	//m_cactus->GetModelRender().PlayAnimation(enAninationClip_Patrol, 0.2f);
}
