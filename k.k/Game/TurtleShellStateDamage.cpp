#include "stdafx.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShell.h"

void TurtleShellStateDamage::ManageState()
{
	m_turtleShell->OnProcessDamageStateTransition();
}

void TurtleShellStateDamage::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Hit);
}
