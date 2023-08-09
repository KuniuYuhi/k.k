#include "stdafx.h"
#include "TurtleShellStateDifenceDamage.h"
#include "TurtleShell.h"

void TurtleShellStateDifenceDamage::ManageState()
{
	m_turtleShell->OnProcessDifenceDamageStateTransition();
}

void TurtleShellStateDifenceDamage::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_DefenceDamage);
}
