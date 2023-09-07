#include "stdafx.h"
#include "TurtleShellStateAppear.h"
#include "TurtleShell.h"

void TurtleShellStateAppear::ManageState()
{
	m_turtleShell->OnProcessAppearStateTransition();
}

void TurtleShellStateAppear::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_Appear);
}
