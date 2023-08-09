#include "stdafx.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShell.h"

void TurtleShellStateAttack_1::ManageState()
{
	m_turtleShell->OnProcessAttack_1StateTransition();
}

void TurtleShellStateAttack_1::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(TurtleShell::enAnimClip_Attack_1);
}
