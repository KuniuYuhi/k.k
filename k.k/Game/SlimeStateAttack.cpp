#include "stdafx.h"
#include "SlimeStateAttack.h"
#include "Slime.h"

void SlimeStateAttack::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
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
