#include "stdafx.h"
#include "MobMonsterSM_Patrol.h"
#include "MobMonster.h"

using namespace MobMonsterInfo;

void MobMonsterSM_Patrol::Execute()
{
	//�s�������߂�
	ProcessDecideAction();
}

void MobMonsterSM_Patrol::ProcessDecideAction()
{
	//���̃A�j���[�V�������͏������Ȃ�
	if (m_mobMonster->IsAttackEnable() != true)
	{
		return;
	}
	if (m_mobMonster->isAnimationEnable() != true)
	{
		return;
	}

	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//�v���C���[����������ǂ�������̂ŃX�e�[�g�}�V����؂�ւ���
		m_mobMonster->SetNextStateMachine(enStateMachineState_Chase);
	}
	else
	{
		//�v���C���[�������Ă��Ȃ��Ȃ�p�g���[��or�ҋ@
		ProcessPatrol();
	}
}

void MobMonsterSM_Patrol::ProcessPatrol()
{


	//���񂷂�
	m_mobMonster->SetNextAnimationState(enAninationState_Patrol);
}
