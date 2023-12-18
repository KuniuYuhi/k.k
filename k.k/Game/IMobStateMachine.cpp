#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"

//�A�j���[�V�����X�e�[�g���ꂵ�Ă��悢����
using namespace MobMonsterInfo;

//				
//
//				�ǂ�������	���\�\�\�\�\�\�\�\�\��	����
//					|								  |
//					|								  |		
//				   �U��								�ҋ@
//					|
//		�ʏ�U���\�\�\�\�\�X�L��				 
// 


IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
}

void IMobStateMachine::Execute()
{
	//�s�������߂�
	ProcessDecideAction();
}

void IMobStateMachine::ProcessDecideAction()
{
	//���̃A�j���[�V�������͏������Ȃ�
	if (m_mobMonster->IsAttackEnable()!=true)
	{
		return;
	}
	if (m_mobMonster->isAnimationEnable() != true)
	{
		return;
	}


	//���ꂩ��s�������߂�
	DecideNextAction();
}

void IMobStateMachine::DecideNextAction()
{
	//�v���C���[��������������

	//todo ���Ƀv���C���[�̎���ɓG�������̂���Ȃ�߂��Ȃ�

	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//�v���C���[����������ǂ�������
		ProcessChase();
	}
	else
	{
		//�v���C���[�������Ă��Ȃ��Ȃ�p�g���[��or�ҋ@
		ProcessPatrol();
	}

}

void IMobStateMachine::ProcessPatrol()
{
	//���񂷂�
	m_mobMonster->SetNextAnimationState(enAninationState_Patrol);
}

void IMobStateMachine::ProcessChase()
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

void IMobStateMachine::ProcessAttackState()
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

	

