#include "stdafx.h"
#include "TurtleShellStateChase.h"
#include "TurtleShell.h"

void TurtleShellStateChase::ManageState()
{
	//ƒvƒŒƒCƒ„[‚ð’Ç‚¢‚©‚¯‚é
	m_turtleShell->MoveChasePlayer(m_turtleShell->GetCharacterController());
}

void TurtleShellStateChase::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Chase, 0.2f);
}
