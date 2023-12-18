#include "stdafx.h"
#include "CactusStateAttack_1.h"
#include "Cactus.h"

void CactusStateAttack_1::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_cactus->GetCreateAttackCollisionFlag() == true)
	{
		m_cactus->CreateCollision();
	}

	m_cactus->OnProcessAttack_1StateTransition();
}

void CactusStateAttack_1::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Attack);
}
