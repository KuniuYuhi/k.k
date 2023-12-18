#include "stdafx.h"
#include "SlimeStateSkill.h"
#include "Slime.h"

void SlimeStateSkill::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
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
