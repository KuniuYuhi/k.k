#include "stdafx.h"
#include "SummonerSSM_General.h"
#include "SummonerSMContext.h"
#include "Summoner.h"

#include "EnemyManager.h"

void SummonerSSM_General::Entry()
{
	//�T���i�[��HP��ۑ�
	m_summonerHp = m_summoner->GetCommonStatus().GetCurrentHp();
}

void SummonerSSM_General::Update()
{
	if (m_summoner->IsAction())
	{
		return;
	}
	

	//�󋵂ɂ���ăX�e�[�g��؂�ւ���
	//�X�e�[�g��؂�ւ����ꍇ�͐�̏��������Ȃ�
	if (EvaluateSituation()) return;


	//�U���X�e�[�g�}�V���ɐ؂�ւ��邩�`�F�b�N
	CheckEnableChangeAttackStateMachine();


	//�ړ����@���`�F�b�N
	CheckMovePattern();


}

void SummonerSSM_General::Exit()
{
}

void SummonerSSM_General::CalcStateMachineChangeTimer()
{
	if (m_stateMachineChangeTimer >= m_stateMachineChangeTimeLimit)
	{
		//�X�e�[�g�}�V���؂�ւ��\
		m_isEnableChangeStateMachine = true;
		return;
	}
	//�^�C�}�[�����Z
	m_stateMachineChangeTimer += g_gameTime->GetFrameDeltaTime();
}

void SummonerSSM_General::CheckMovePattern()
{
	//�ړ�����̂���߂�

	//�^�C�}�[�̌v�Z
	CalcMovePatternCheckTime();

	//�t���O���������Ȃ�
	if (m_isMovePatternCheck)
	{
		int ram = rand() % 10;

		//�v���C���[�Ƃ̋����������قǎ~�܂�m�����オ��

		float distance = m_summoner->GetDistanceToPlayerPositionValue();
		//���`��Ԃŋ��������Z����m���ɕϊ�
		//���Z����m���͍ő�3�B�����̕�Ԃ�0�`1000
		int add = Math::Lerp((distance - 0.0f) / (1000.0f - 0.0f), 0.0f, 3.0f);

		if (ram < (5 + add))
		{
			//�ړ����~�߂�
			m_summoner->SetStopMoveFlag(true);
		}
		else
		{
			//�ړ����~�߂Ȃ�
			m_summoner->SetStopMoveFlag(false);
		}

		//�t���O���Z�b�g
		m_isMovePatternCheck = false;
		//�^�C�}�[�̃��Z�b�g
		m_movePatternCheckTimer = 0.0f;
	}
}

void SummonerSSM_General::CalcMovePatternCheckTime()
{
	if (m_movePatternCheckTimer >= m_movePatternCheckTimeLimit)
	{
		//�`�F�b�N�ł���
		m_isMovePatternCheck = true;
		return;
	}
	m_movePatternCheckTimer += g_gameTime->GetFrameDeltaTime();
}

void SummonerSSM_General::CheckEnableChangeAttackStateMachine()
{
	if (m_summoner->IsAction())
	{
		return;
	}

	//�X�e�[�g�}�V���؂�ւ��^�C�}�[���v�Z
	CalcStateMachineChangeTimer();

	//��莞�Ԃ�������U���X�e�[�g�}�V���ɐ؂�ւ���
	if (m_isEnableChangeStateMachine)
	{
		//�t���O�����Z�b�g
		m_isEnableChangeStateMachine = false;
		//�^�C�}�[�����Z�b�g
		m_stateMachineChangeTimer = 0.0f;
		//�U���X�e�[�g�}�V���ɐ؂�ւ�
		m_stateMachineContext->ChangeSubStateMachine(enSubStateMachine_Offensive);
		return;
	}
}

bool SummonerSSM_General::EvaluateSituation()
{
	//�e��N�[���^�C���p�^�C�}�[
	AddTimer(m_warpCoolTimer);

	//�؂�ւ��X�e�[�g
	EnSummonerState changeState = enSummonerState_None;

	//���[�v�X�e�[�g�ɐ؂�ւ��邩�`�F�b�N
	if (CheckChangeWarpState())
	{
		changeState = enSummonerState_Warp;
	}



	//�؂�ւ������X�e�[�g������Ȃ�
	if (changeState != enSummonerState_None)
	{
		//�X�e�[�g��؂�ւ���
		m_summoner->ChangeState(changeState);
		//�O��̃X�e�[�g��ݒ�
		m_stateMachineContext->SetPreviousState(changeState);

		return true;
	}


	return false;

}

bool SummonerSSM_General::CheckChangeWarpState()
{
	//�O������[�v�����Ȃ烏�[�v���Ȃ�
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_Warp) return false;
	//�^�C�}�[����莞�Ԍo���Ă��Ȃ��Ȃ烏�[�v���Ȃ�
	if (m_warpCoolTimer < m_warpCoolTimeLimit) return false;

	//�X�R�A
	int score = 0;

	//HP�̊������v�Z
	float ratio = (float)m_summoner->GetCommonStatus().GetCurrentHp() / (float)m_summonerHp;
	//1.5���ȏ���ꂽ��
	ratio = 1 - ratio;
	if (ratio > 0.13f)
	{
		//�������g���ăX�R�A���Z�B������HP�������Ⴂ�قǃX�R�A���傫���Ȃ�
		score += 200 * ratio;
	}

	//���ɋ��������ȏ㗣��Ă���Ȃ烏�[�v���Ȃ�
	float length = m_summoner->GetDistanceToPlayerPositionValue();
	//�������߂��Ȃ�X�R�A�����Z
	if (length < 700.0f)
	{
		score += 20;
	}

	//�{�X�̓���̔��a�̒��ɂ��郂�u�G�l�~�[�̐����擾
	int mobEnemyAmount = EnemyManager::GetInstance()->GetNearbyEnemyCount(
		m_summoner->GetPosition(),600.0f
	);

	score += mobEnemyAmount * 3;


	//���v�̏d�݂����ȏ�Ȃ�

	if (score < 60) return false;

	//�^�C�}�[�����Z�b�g
	m_warpCoolTimer = 0.0f;
	return true;
}








void SummonerSSM_General::AddTimer(float& timer)
{
	timer += g_gameTime->GetFrameDeltaTime();
}
