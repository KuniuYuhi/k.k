#include "stdafx.h"
#include "SummonerSM_Vigilance.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float WAIT_TIME = 500.0f;		//�ҋ@����

	const float MELEE_ATTACK_RANGE = 280.0f;	//�ߋ����U���͈͓̔�

	const float STAY_PLAYER_LIMMIT_TIME = 500.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

}

SummonerSM_Vigilance::~SummonerSM_Vigilance()
{
}

void SummonerSM_Vigilance::Execute()
{
	ProcessDecideAction();
}

void SummonerSM_Vigilance::Init(bool saveTimerlesetFlag)
{
	if (saveTimerlesetFlag == true)
	{
		//�^�C�}�[�����Z�b�g
		m_stayPlayerTimer = 0.0f;
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
	//�^�C�}�[�����Z
	m_waitTimer += g_gameTime->GetFrameDeltaTime();

	//�m�b�N�o�b�N���邩���߂�A���܂����瑦�m�b�N�o�b�N
	ProcessIsKnockBack();
}

void SummonerSM_Vigilance::ProcessIsKnockBack()
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

		//�m�b�N�o�b�N����
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_KnockBack);
		return;

	}
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
