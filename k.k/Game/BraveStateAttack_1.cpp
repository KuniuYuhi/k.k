#include "stdafx.h"
#include "BraveStateAttack_1.h"
#include "Brave.h"

void BraveStateAttack_1::ManageState()
{
	//�O�i����U���t���O�������Ă���Ԃ͏���
	if (m_brave->GetDashAttackFlag() == true)
	{
		//�����Ă�������Ɉړ�
		m_brave->CalcAttackDirection(m_brave->GetNormalAttackSpeed());
	}

	m_brave->ProcessNormalAttackStateTransition();
	
}

void BraveStateAttack_1::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_attack1, 0.2f);
}


