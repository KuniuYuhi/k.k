#include "stdafx.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShell.h"

void TurtleShellStateAttack_1::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
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
