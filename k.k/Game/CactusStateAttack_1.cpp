#include "stdafx.h"
#include "CactusStateAttack_1.h"
#include "Cactus.h"

void CactusStateAttack_1::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
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
