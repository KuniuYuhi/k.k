#include "stdafx.h"
#include "TurtleShellStateVictory.h"
#include "TurtleShell.h"

void TurtleShellStateVictory::ManageState()
{
	m_turtleShell->OnProcessVictoryStateTransition();
}

void TurtleShellStateVictory::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Victory);
}
