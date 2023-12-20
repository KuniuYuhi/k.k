#include "stdafx.h"
#include "TurtleShellStatePatrol.h"
#include "TurtleShell.h"

void TurtleShellStatePatrol::ManageState()
{
	m_turtleShell->MovePatrol(m_turtleShell->GetCharacterController());
}

void TurtleShellStatePatrol::PlayAnimation()
{
	if (fabsf(m_turtleShell->GetMoveSpeed().x) >= 0.001f || fabsf(m_turtleShell->GetMoveSpeed().z) >= 0.001f)
	{
		m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Patrol, 0.2f);
	}
	else
	{
		m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Idle, 0.4f);
	}
}
