#include "stdafx.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShell.h"

void TurtleShellStateAttack_1::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
	if (m_turtleShell->GetCreateAttackCollisionFlag() == true)
	{
		m_turtleShell->CreateCollision();
	}

	m_turtleShell->OnProcessAttack_1StateTransition();
}

void TurtleShellStateAttack_1::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Attack);
}
