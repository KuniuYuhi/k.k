#include "stdafx.h"
#include "TurtleShellStateWalk.h"
#include "TurtleShell.h"

void TurtleShellStateWalk::ManageState()
{
	m_turtleShell->ProcessCommonStateTransition();
}

void TurtleShellStateWalk::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_Walk);
}
