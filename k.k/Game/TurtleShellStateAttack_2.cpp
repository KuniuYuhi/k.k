#include "stdafx.h"
#include "TurtleShellStateAttack_2.h"
#include "TurtleShell.h"

void TurtleShellStateAttack_2::ManageState()
{
	m_turtleShell->OnProcessAttack_2StateTransition();
}

void TurtleShellStateAttack_2::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_Attack_2);
}
