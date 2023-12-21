#include "stdafx.h"
#include "TurtleShellStateIdle.h"
#include "TurtleShell.h"

void TurtleShellStateIdle::ManageState()
{
	m_turtleShell->ProcessCommonStateTransition();
}

void TurtleShellStateIdle::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Idle,0.3f);
}
