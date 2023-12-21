#include "stdafx.h"
#include "CactusStateSkill.h"
#include "Cactus.h"

void CactusStateSkill::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_cactus->GetCreateAttackCollisionFlag() == true)
	{
		m_cactus->CreateCollision();
	}

	m_cactus->OnProcessAttack_1StateTransition();
}

void CactusStateSkill::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Skill);
}
