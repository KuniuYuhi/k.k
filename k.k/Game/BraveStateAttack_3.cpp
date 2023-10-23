#include "stdafx.h"
#include "BraveStateAttack_3.h"
#include "Brave.h"

void BraveStateAttack_3::ManageState()
{
	//�O�i����t���O�������Ă���Ԃ͏���
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//�����Ă�������Ɉړ�
		m_brave->MoveForward(m_brave->GetNormalAttackSpeed());
	}

	m_brave->ProcessNormalAttackStateTransition();
}

void BraveStateAttack_3::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack3 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}
