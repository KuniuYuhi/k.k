#include "stdafx.h"
#include "MobMonsterSM_Chase.h"
#include "MobMonster.h"

using namespace MobMonsterInfo;

void MobMonsterSM_Chase::Execute()
{
	//�s�������߂�
	ProcessDecideAction();
}

void MobMonsterSM_Chase::ProcessDecideAction()
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

	DecideNextAction();
}

void MobMonsterSM_Chase::DecideNextAction()
{
	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//�v���C���[����������ǂ�������
		ProcessChase();
	}
	else
	{
		//�v���C���[������������X�e�[�g�}�V����؂�ւ���
		m_mobMonster->SetNextStateMachine(enStateMachineState_Patrol);
	}

}

void MobMonsterSM_Chase::ProcessAttackState()
{
	//�܂��U�����ł��邩
	if (m_mobMonster->IsProcessAttackEnable() == true)
	{
		//�X�L���U�����ł����ԂȂ�
		if (m_mobMonster->IsSkillUsable() == true)
		{
			//�X�L���U��
			m_mobMonster->SetNextAnimationState(enAnimationState_Skill);
		}
		else
		{
			//�ʏ�U��
			m_mobMonster->SetNextAnimationState(enAnimationState_Attack);
		}
	}
	else
	{
		//�U���ł��Ȃ��Ȃ炻�̏�őҋ@
		m_mobMonster->SetNextAnimationState(enAninationState_Idle);
	}
}

void MobMonsterSM_Chase::ProcessChase()
{
	//�U���͈͓��Ƀv���C���[������Ȃ�
	if (m_mobMonster->IsPlayerInAttackRange() == true)
	{
		//�U�����@�����߂�
		ProcessAttackState();
	}
	else
	{
		//�ǂ�������
		m_mobMonster->SetNextAnimationState(enAninationState_Chase);
	}

}