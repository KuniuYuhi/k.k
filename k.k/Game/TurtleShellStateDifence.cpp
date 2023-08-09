#include "stdafx.h"
#include "TurtleShellStateDifence.h"
#include "TurtleShell.h"

void TurtleShellStateDifence::ManageState()
{
	m_turtleShell->OnProcessDifenceStateTransition();
}

void TurtleShellStateDifence::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_Difence);
}
