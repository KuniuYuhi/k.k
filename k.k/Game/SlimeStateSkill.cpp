#include "stdafx.h"
#include "SlimeStateSkill.h"
#include "Slime.h"

void SlimeStateSkill::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_slime->GetCreateAttackCollisionFlag() == true)
	{
		m_slime->CreateCollision();
	}

	m_slime->OnProcessAttack_1StateTransition();
}

void SlimeStateSkill::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Skill);
}
