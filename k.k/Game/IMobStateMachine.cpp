#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"

//�A�j���[�V�����X�e�[�g���ꂵ�Ă��悢����

IMobStateMachine* IMobStateMachine::m_stateMachineInstance = nullptr;

IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
	m_stateMachineInstance = nullptr;
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


	//���ꂩ��s�������߂�
	DecideNextAction();
}

void IMobStateMachine::DecideNextAction()
{
	//�v���C���[��������������
	if (m_mobMonster->IsFoundPlayer() == true)
	{
		//�v���C���[����������ǂ�������
		ProcessChase();
	}
	else
	{
		//�v���C���[�������Ă��Ȃ��Ȃ�p�g���[��
		ProcessPatrol();
	}
	
}

void IMobStateMachine::ProcessPatrol()
{

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
	}
}

void IMobStateMachine::ProcessAttackState()
{

}

