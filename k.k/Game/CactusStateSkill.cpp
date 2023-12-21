#include "stdafx.h"
#include "CactusStateSkill.h"
#include "Cactus.h"

void CactusStateSkill::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
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
