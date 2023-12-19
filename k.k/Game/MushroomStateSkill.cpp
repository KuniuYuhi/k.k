#include "stdafx.h"
#include "MushroomStateSkill.h"
#include "Mushroom.h"

void MushroomStateSkill::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_mushroom->GetCreateAttackCollisionFlag() == true)
	{
		m_mushroom->CreateCollision();
	}

	m_mushroom->OnProcessAttack_1StateTransition();
}

void MushroomStateSkill::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Skill);
}
