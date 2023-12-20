#include "stdafx.h"
#include "TurtleShellStateChase.h"
#include "TurtleShell.h"

void TurtleShellStateChase::ManageState()
{
	//�v���C���[��ǂ�������
	m_turtleShell->MoveChasePlayer(m_turtleShell->GetCharacterController());
}

void TurtleShellStateChase::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Chase, 0.2f);
}
