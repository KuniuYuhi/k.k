#include "stdafx.h"
#include "TurtleShellStateDie.h"
#include "TurtleShell.h"

void TurtleShellStateDie::ManageState()
{
	m_turtleShell->OnProcessDieStateTransition();
}

void TurtleShellStateDie::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Die);
}
