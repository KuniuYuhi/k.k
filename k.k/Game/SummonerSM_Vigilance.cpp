#include "stdafx.h"
#include "SummonerSM_Vigilance.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float WAIT_TIME = 50.0f;		//�ҋ@����

	const float MELEE_ATTACK_RANGE = 280.0f;	//�ߋ����U���͈͓̔�

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��


	const float KNOCKBACK_DISTANCE = 300.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

	//�^�C�}�[��0�ɂ��Ȃ�
	const float START_DECIDE_ACTION_TIMER = 1.0f;

	const float DEFAULT_FIND_PLAYER_DISTANCE = 400.0f;

	const float ADD_DISTANCE = 50.0f;

	const float DECIDE_ACTION_TIMER_LIMMIT = 5.0f;

}

float SummonerSM_Vigilance::m_decideActionTimer = START_DECIDE_ACTION_TIMER;

SummonerSM_Vigilance::~SummonerSM_Vigilance()
{
}

void SummonerSM_Vigilance::Execute()
{
	//�������~�߂Ă���Ȃ珈�������Ȃ�
	if (m_summoner->IsStopProcessing() == true)
	{
		return;
	}

	if (m_summoner->isAnimationEnable() != true)
	{
		return;
	}

	//�s�������߂�
	ProcessDecideAction();
}

void SummonerSM_Vigilance::Init(bool saveTimerlesetFlag)
{
	//�v���C���[��������������Ƃ�ϐ��̏�����
	m_FindPlayerDistance = DEFAULT_FIND_PLAYER_DISTANCE;

	if (saveTimerlesetFlag == true)
	{
		//�e��^�C�}�[�����Z�b�g
		m_stayPlayerTimer = 0.0f;

		m_decideActionTimer = START_DECIDE_ACTION_TIMER;
	}	
}

void SummonerSM_Vigilance::ProcessDecideAction()
{
	//�܂��ҋ@�X�e�[�g���U���X�e�[�g�����߂�
	//�ҋ@���Ԃ̊Ԃ܂��͂�����x�U������`�����X���Ȃ��Ȃ�
	if (m_waitTimer < WAIT_TIME)
	{
		//�ҋ@�X�e�[�g�����߂�
		ProcessWaitState();
	}
	else
	{
		//�U���X�e�[�g�}�V���ɐ؂�ւ���
		m_summoner->SetNextStateMachine(Summoner::enStateMachineState_Attack);
		return;
	}
}

void SummonerSM_Vigilance::ProcessWaitState()
{
	//�ҋ@�^�C�}�[�����Z
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//�m�b�N�o�b�N���邩���߂�A���܂����瑦�m�b�N�o�b�N
	if (IsKnockBack() == true)
	{
		//�m�b�N�o�b�N�A�j���[�V�����X�e�[�g
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;
	}
	


	//�ǂ������邩���̏�őҋ@���邩���߂�
	if (IsChasePlayer() == true)
	{
		//�����A�j���[�V�����X�e�[�g
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Walk);
		return;
	}
	else
	{
		//�ҋ@�A�j���[�V�����X�e�[�g
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Idle);
		return;
	}
	
}

bool SummonerSM_Vigilance::IsKnockBack()
{
	//�T���i�[����E�҂Ɍ������x�N�g�����v�Z����
	m_toPlayer = m_summoner->GetPosition() -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();


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
		//�Ƃǂ܂��Ă���^�C�}�[���Z�b�g
		m_stayPlayerTimer = 0.0f;
		//�m�b�N�o�b�N�����シ���U�����Ȃ��悤�Ɉ�b�͋󂯂�
		if (m_waitTimer > 1.0f)
		{
			m_waitTimer = 1.0f;
		}

		return true;

	}

	return false;
}

void SummonerSM_Vigilance::IsKnockBackTimerAccelerate()
{
}

void SummonerSM_Vigilance::AddStayPlayerTimer()
{
	//�����ɂ���ă^�C�}�[������������
	IsKnockBackTimerAccelerate();
	//�^�C�}�[�����Z
	m_stayPlayerTimer += g_gameTime->GetFrameDeltaTime() * m_accelerateStayPlayerTimer;



	//���炩�̗v���ł���Ƀ^�C�}�[������-��m_accelerateStayPlayerTimer
}

bool SummonerSM_Vigilance::IsChasePlayer()
{
	//���̋������Ƀv���C���[������Ȃ�m��őҋ@
	if (m_summoner->IsFindPlayer(m_FindPlayerDistance) == true)
	{
		//�������Z�t���O���Z�b�g����Ă���Ȃ�
		if (m_addDsitanceFlag == true)
		{
			//�ړ��Ƒҋ@�����݂ɂȂ��ăK�^�K�^�Ȃ�΍�
			m_FindPlayerDistance += ADD_DISTANCE;
			//�������Z�t���O�����Z�b�g����
			//�ҋ@���Ă���Ԃ͋������L�тȂ��悤�ɂ���
			m_addDsitanceFlag = false;
		}
		//�ҋ@��Ԃ�Ԃ�
		return false;
	}

	//�s������^�C�}�[���������ԂɒB����܂ł́A�O�Ɠ����s�����Ƃ�
	if (m_decideActionTimer < DECIDE_ACTION_TIMER_LIMMIT)
	{
		//��x�s�������܂������莞�Ԃ��̂܂�
		//�s������^�C�}�[�����Z
		m_decideActionTimer += g_gameTime->GetFrameDeltaTime();
		//�������Z�t���O�����Z�b�g
		m_addDsitanceFlag = true;
	}
	else
	{
		//�s������^�C�}�[�����Z�b�g
		m_decideActionTimer = 0.0f;

		//�m���őҋ@�ɂ���
		int probability = rand() % 10;
		//�m�������l���傫�������炻�̏�őҋ@
		if (probability > 4)
		{
			m_saveStateFlag = false;
			//�ҋ@��Ԃ�Ԃ�
			return m_saveStateFlag;
		}
		else
		{
			m_saveStateFlag = true;
			//�ړ���Ԃ�Ԃ�
			return m_saveStateFlag;
		}
	}

	//�����ɗ�����O�t���[���̍s���Ɠ�����Ԃ�Ԃ�
	return m_saveStateFlag;
	
}
