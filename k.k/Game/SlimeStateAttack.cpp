#include "stdafx.h"
#include "SlimeStateAttack.h"
#include "Slime.h"

void SlimeStateAttack::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_slime->GetCreateAttackCollisionFlag() == true)
	{
		m_slime->CreateCollision();
	}

	m_slime->OnProcessAttack_1StateTransition();
}

void SlimeStateAttack::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Attack);
}
