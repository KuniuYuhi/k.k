#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()�̈����ɃI�[�_�[�������

namespace {
	const float WAIT_TIME = 5.0f;		//�ҋ@����


	const float STAY_PLAYER_LIMMIT_TIME = 6.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��
}

IBossStateMachine* IBossStateMachine::m_stateMachineInstance = nullptr;

IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;
}

IBossStateMachine::~IBossStateMachine()
{
	m_stateMachineInstance = nullptr;
}

void IBossStateMachine::Execute()
{


	//�s�������߂�
	ProcessDecideAction();

}

void IBossStateMachine::ProcessDecideAction()
{
	//�ǂ�ȍU�������邩���߂�
	//		�U��			�ҋ@
	//		�b
	//		�b
	//�ߋ����@������
	//	�b		�b
	//

	//���Ԋu�ōU���B��������őҋ@��Ԃł��U���Ɉڍs
	//�T���i�[���U�����͏������Ȃ�
	if (m_summoner->IsAttackEnable() == false)
	{
		return;
	}
	//���ꂩ��s�������߂�
	DecideNextAction();
	//�s���ɂ���ĘA���ŃA�N�V�������邩���߂�
}

void IBossStateMachine::DecideNextAction()
{
	//�A���U������Ȃ璊�I����

	//�܂��ҋ@�X�e�[�g���U���X�e�[�g�����߂�
	//�ҋ@���Ԃ̊Ԃ܂��͂�����x�U������`�����X���Ȃ��Ȃ�
	if (m_waitTimer < WAIT_TIME&&m_lotteryChanceFlag == false)
	{
		//�ҋ@�X�e�[�g�����߂�
		ProcessWaitState();
	}
	else
	{
		//�U���X�e�[�g�����߂�
		ProcessAttackState();
	}
}

void IBossStateMachine::ProcessAttackState()
{
	//�ǂ�ȍU���ɂ��邩���߂�

	//�T���i�[����E�҂Ɍ������x�N�g�����v�Z����
	m_toPlayer = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetBraveInstance()->GetPosition();

	//�����ɂ���ċߋ����U�����������U�������߂�
	if (m_toPlayer.Length() > 200.0f)
	{
		//�������U��
		ProcessLongRangeAttack();
	}
	else
	{
		//�ߋ����U��
		ProcessMeleeAttack();
	}

	//�s��������̏���
	m_waitTimer = 0.0f;
	//�N�������A�N�V�������L��
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void IBossStateMachine::ProcessWaitState()
{
	//�^�C�}�[�����Z
	m_waitTimer += g_gameTime->GetFrameDeltaTime();


	//�����ԗE�҂��߂��ɂ���Ȃ�
	//�����邩�m�b�N�o�b�N�ň�������
	if (m_toPlayer.Length() < 200.0f)
	{
		AddStayPlayerTimer();
	}
	else
	{
		//�͈͊O�Ȃ�^�C�}�[���Z�b�g
		m_stayPlayerTimer = 0.0f;
	}
	//�Ƃǂ܂��Ă��鎞�Ԃ��������������
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		//�����I�ɍU���X�e�[�g���߂Ɉڂ�
		//�ߋ����U��
		ProcessMeleeAttack();
		//�Ƃǂ܂��Ă���^�C�}�[���Z�b�g
		m_stayPlayerTimer = 0.0f;
		//wait�^�C�}�[���Z�b�g����H
	}



	//�O�̍s�����_�[�N�{�[���Ȃ�
	//if (m_oldActionName == enActionName_Darkball&& m_lotteryChance>0)
	//{
	//	SetRandomActionValue();
	//	m_lotteryChance--;
	//}

	////������x�s������x�Ɋm����������
	//if (m_randomValue > attackRetryProbability)
	//{
	//	//������x�s������̂őҋ@���Ԃ̒Z�k
	//	m_waitTimer = WAIT_TIME;
	//	DecideNextAction();
	//	return;
	//}
}

void IBossStateMachine::ProcessLongRangeAttack()
{
	

	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_DarkBall);

	int chance = rand() % 10;
	if (chance > attackRetryProbability && m_lotteryChanceFlag == false)
	{
		//������x�U������`�����X����
		m_lotteryChanceFlag = true;
		/*attackRetryProbability *= 2;
		if (attackRetryProbability > 10)
			attackRetryProbability = 10;*/
		return;
	}

	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

void IBossStateMachine::ProcessMeleeAttack()
{
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}

	//�ߋ����U���J�E���g�𑝂₷
	m_meleeCount++;

	//���g�̎��͂Ƀ��u�����Ȃ��A�����ԋ߂��ɂ���Ȃ�m�b�N�o�b�N
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;
	}

	int bb = rand() % 10;

	if (bb > 4)
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
	}
	else
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	}
	
}

void IBossStateMachine::AddStayPlayerTimer()
{
	//�^�C�}�[�����Z
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;
	//���炩�̗v���ł���Ƀ^�C�}�[������-��m_accelerateStayPlayerTimer
	




}
