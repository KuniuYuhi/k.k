#include "stdafx.h"
#include "TurtleShellStateChase.h"
#include "TurtleShell.h"

void TurtleShellStateChase::ManageState()
{
	//プレイヤーを追いかける
	m_turtleShell->MoveChasePlayer(m_turtleShell->GetCharacterController());
}

void TurtleShellStateChase::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Chase, 0.2f);
}
