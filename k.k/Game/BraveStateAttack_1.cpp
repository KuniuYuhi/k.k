#include "stdafx.h"
#include "BraveStateAttack_1.h"
#include "Brave.h"

void BraveStateAttack_1::ManageState()
{
	//���s���������炱��ȏ㓮���Ȃ��悤�ɂ���
	if (m_brave->IsDecisionOutcomeForState() == true)
	{
		return;
	}

	//�O�i����t���O�������Ă���Ԃ͏���
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//�����Ă�������Ɉړ�
		//����̃X�s�[�h�ɂ���
		m_brave->MoveForward(m_brave->GetWeapon(enMaxWeapons_Main)->GetMoveForwardSpeed());
	}
	//��ނ���t���O�������Ă����珈������
	else if (m_brave->GetMoveBackFlag() == true)
	{
		//�����Ă�������Ɉړ�
		//����̃X�s�[�h�ɂ���
		m_brave->MoveBack(m_brave->GetWeapon(enMaxWeapons_Main)->GetMoveBackSpeed());
	}


	m_brave->ProcessNormalAttackStateTransition();
	
}

void BraveStateAttack_1::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_attack1 + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);
}


