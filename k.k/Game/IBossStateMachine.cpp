#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "Player.h"
#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()�̈����ɃI�[�_�[�������

namespace {
	const float WAIT_TIME = 2.0f;		//�ҋ@����

	const float MELEE_ATTACK_RANGE = 280.0f;	//�ߋ����U���͈͓̔�

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 1.0f;

}

IBossStateMachine* IBossStateMachine::m_stateMachineInstance = nullptr;

IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;

	//�������������B
	srand((unsigned)time(NULL));
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
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();

	//�����ɂ���ċߋ����U�����������U�������߂�
	if (m_toPlayer.Length() < MELEE_ATTACK_RANGE)
	{
		//�ߋ����U��
		ProcessMeleeAttack();
	}
	else
	{
		//�������U��
		ProcessLongRangeAttack();
	}

	//�{�胂�[�h�J�E���g���Z
	//m_angryCount++;

	//�s��������̏���
	m_waitTimer = 0.0f;
	//�N�������A�N�V�������L��
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void IBossStateMachine::ProcessWaitState()
{
	//�^�C�}�[�����Z
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//�{�胂�[�h
	//if (m_summoner->GetNowSpecialActionState() !=
	//	Summoner::enSpecialActionState_AngryMode)
	//{
	//	if (m_angryCount > 3)
	//	{
	//		m_summoner->
	//			SetNextAnimationState(Summoner::enAnimationState_Angry);
	//		//�^�C�}�[���Z�b�g
	//		m_angryCount = 0;
	//		return;
	//	}
	//}
	
	//�m�b�N�o�b�N���邩���߂�A���܂����瑦�m�b�N�o�b�N
	ProcessIsKnockBack();

}

void IBossStateMachine::ProcessLongRangeAttack()
{
	//�������U��

	//�_�[�N���e�I
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_Attack_DarkMeteorite_start);
	return;

	//�O�̍s�����_�[�N���e�I�łȂ��Ȃ�
	//�_�[�N���e�I���������ׂ�
	if (IsNextActionDarkMeteo() == true&& m_oldActionName != enActionName_DarkMeteo_Start)
	{
		//�_�[�N���e�I
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Attack_DarkMeteorite_start);
		return;
	}

	//�_�[�N�{�[��
	ProcessPlayAnimDarkBall();

	//���ȏ�A���U���͂��Ȃ�
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

void IBossStateMachine::ProcessMeleeAttack()
{
	//�ߋ����U��

	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}

	//�ߋ����U���J�E���g�𑝂₷
	m_meleeCount++;

	//���g�̎��͂Ƀ��u�����Ȃ��A�����ԋ߂��ɂ���Ȃ�m�b�N�o�b�N
	if (m_stayPlayerTimer > STAY_PLAYER_LIMMIT_TIME)
	{
		//�m�b�N�o�b�N
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		//�m�b�N�o�b�N�����^�C�}�[���Z�b�g
		m_accelerateStayPlayerTimer = 1.0f;
		return;
	}

	int bb = rand() % 10;
	//�m���ŕω�
	if (bb > 4)
	{
		//�_�[�N�E�H�[��
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
	}
	else
	{
		//�ʏ�U��
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	}
	
}

void IBossStateMachine::AddStayPlayerTimer()
{
	//�����ɂ���ă^�C�}�[������������
	IsKnockBackTimerAccelerate();
	//�^�C�}�[�����Z
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;
	//���炩�̗v���ł���Ƀ^�C�}�[������-��m_accelerateStayPlayerTimer
}

void IBossStateMachine::ProcessPlayAnimDarkBall()
{
	//��A���Ō�������A�ǔ�����_�[�N�{�[���ɂ����肷��
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

	//���ȏ�A���U���͂��Ȃ�
	if (m_lotteryChanceFlag == true)
	{
		m_lotteryChanceFlag = false;
	}
}

bool IBossStateMachine::IsNextActionDarkMeteo()
{
	//���̃|�C���g�����ȏ�ɂȂ�����A���̍s���̓_�[�N���e�I�ɂ���
	float point = 0;

	//�v���C���[�̍��W
	Vector3 playerPos = CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//����Ă��鋗�����|�C���g�Ɋ��Z����
	Vector3 diff = m_summoner->GetPosition() - playerPos;
	//�|�C���g�����Z�B�x�N�g���̏\���̈�̒��������Z
	point += (diff.Length() / 100);

	//�v���C���[�̎���Ƀ��u�����X�^�[���ǂ̂��炢���Ȃ����m���߂�
	int count = 0;
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		Vector3 mobToPlayer = playerPos - monster->GetPosition();
		//�v���C���[������̋������Ƀ����X�^�[�����Ȃ��Ȃ�
		if (mobToPlayer.Length() > 300.0f)
		{
			//�J�E���g�����Z
			count++;
		}
	}

	//�͈͓��ɂ��Ȃ��J�E���g���|�C���g�ɉ��Z
	point += count;

	//��_���[�W�J�E���g���|�C���g�ɉ��Z
	point += m_summoner->GetDamageCount();

	//�v�Z�����|�C���g�����l����������
	if (point >= DARK_METEO_ACTION_POINT)
	{
		//�_�[�N���e�I�����̂ŃJ�E���g�����Z�b�g
		m_summoner->SetDamageCount(0);
		return true;
	}
	return false;
}

void IBossStateMachine::ProcessIsKnockBack()
{
	//�����ԗE�҂��߂��ɂ���Ȃ�
	//�����邩�m�b�N�o�b�N�ň�������
	if (m_toPlayer.Length() < KNOCKBACK_DISTANCE)
	{
		//�^�C�}�[�����Z�B�U���������肷��ƃ^�C�}�[������ɉ���
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
		//�m�b�N�o�b�N������̍U���͈�b�͋󂯂�
		if (m_waitTimer > 1.0f)
		{
			m_waitTimer = 1.0f;
		}
	}
}

void IBossStateMachine::IsKnockBackTimerAccelerate()
{

	//if()
}
