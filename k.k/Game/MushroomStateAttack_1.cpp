#include "stdafx.h"
#include "MushroomStateAttack_1.h"
#include "Mushroom.h"

void MushroomStateAttack_1::ManageState()
{
	//�����蔻�萶���t���O���Z�b�g����Ă����瓖���蔻�萶��
	if (m_mushroom->GetCreateAttackCollisionFlag() == true)
	{
		m_mushroom->CreateCollision();
	}

	m_mushroom->OnProcessAttack_1StateTransition();
}

void MushroomStateAttack_1::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Attack);
}
