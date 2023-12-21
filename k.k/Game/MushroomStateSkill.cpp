#include "stdafx.h"
#include "MushroomStateSkill.h"
#include "Mushroom.h"

void MushroomStateSkill::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
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
