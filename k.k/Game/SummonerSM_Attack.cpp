#include "stdafx.h"
#include "SummonerSM_Attack.h"
#include "Summoner.h"
#include "ISummonerState.h"
#include "Player.h"
#include "CharactersInfoManager.h"

namespace {
	const float MELEE_ATTACK_RANGE = 280.0f;	//�ߋ����U���͈͓̔�

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��

	const float DARK_METEO_ACTION_POINT = 30.0f;

}

EnActionName SummonerSM_Attack::m_oldActionName = enActionName_Idle;

void SummonerSM_Attack::Execute()
{
	//�������~�߂Ă���Ȃ珈�������Ȃ�
	if (m_summoner->IsStopProcessing() == true)
	{
		return;
	}

	ProcessDecideAction();
	
}

void SummonerSM_Attack::Init(bool resetoldActionFlag)
{
	if (resetoldActionFlag == true)
	{
		//�O�̃A�N�V���������Z�b�g
		m_oldActionName = enActionName_Idle;
	}
}

void SummonerSM_Attack::ProcessDecideAction()
{
	//���Ԋu�ōU���B��������őҋ@��Ԃł��U���Ɉڍs
	//�T���i�[���U�����͏������Ȃ�
	if (m_summoner->IsAttackEnable() == false)
	{
		return;
	}
	//������x�s������Ƃ���
	if (m_continuousAttackTimer <= 0.0f)
	{
		//���ꂩ��s�������߂�
		DecideNextAction();
	}
	else
	{
		//������x�U���^�C�}�[�����Z
		m_continuousAttackTimer -= g_gameTime->GetFrameDeltaTime();
	}
	
}

void SummonerSM_Attack::DecideNextAction()
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

	//�A���U���񐔂𑝂₷
	m_continuousAttackCount++;
	//�N�������A�N�V�������L��
	m_oldActionName = m_summoner->GetNowSummonerState()->GetActionName();
}

void SummonerSM_Attack::ProcessLongRangeAttack()
{
	//�������U��

	//�_�[�N���e�I
	/*m_summoner->
		SetNextAnimationState(
			Summoner::enAnimationState_Attack_DarkMeteorite_start);
	return;*/

	//�O�̍s�����_�[�N���e�I�łȂ��Ȃ�
	//�_�[�N���e�I���������ׂ�
	if (IsNextActionDarkMeteo() == true && 
		m_oldActionName != enActionName_DarkMeteo_Start)
	{
		//�_�[�N���e�I
		m_summoner->
			SetNextAnimationState(
				Summoner::enAnimationState_Attack_DarkMeteorite_start
			);
	}
	else
	{
		//�_�[�N�{�[��
		IsNextActionDarkBall();
	}

	//�������U���s��������̘A���U���^�C�}�[
	m_continuousAttackTimer = 1.0f;
	
}

void SummonerSM_Attack::ProcessMeleeAttack()
{
	//�ʏ�U��
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
	//�ʏ�U���s��������̘A���U���^�C�}�[
	m_continuousAttackTimer = 0.2f;
	return;
	//�ߋ����U��
	//m_summoner->
	//	SetNextAnimationState(Summoner::enAnimationState_DarkSpear_Start);
	////�_�[�N�X�s�A�U���s��������̘A���U���^�C�}�[
	//m_continuousAttackTimer = 1.5f; 
	//return;

	//�ߋ����U���J�E���g�𑝂₷
	m_meleeCount++;

	int bb = rand() % 10;
	//�m���ŕω�
	if (bb > 7)
	{
		//�_�[�N�X�s�A
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkSpear_Start);
		//�_�[�N�X�s�A�U���s��������̘A���U���^�C�}�[
		m_continuousAttackTimer = 1.5f;
	}
	else if (bb > 4)
	{
		//�_�[�N�E�H�[��
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_DarkWall);
		//�_�[�N�E�H�[���U���s��������̘A���U���^�C�}�[
		m_continuousAttackTimer = 1.0f;
	}
	else
	{
		//�ʏ�U��
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_NormalAttack_1);
		//�ʏ�U���s��������̘A���U���^�C�}�[
		m_continuousAttackTimer = 0.2f;
	}

}

void SummonerSM_Attack::IsNextActionDarkBall()
{
	//��A���Ō�������A�ǔ�����_�[�N�{�[���ɂ����肷��
	m_summoner->
		SetNextAnimationState(Summoner::enAnimationState_DarkBall);

	if (m_lotteryChanceFlag != true)
	{
		//������x�U������`�����X����
		m_lotteryChanceFlag = true;
	}
	else
	{
		//������x�U�����Ă���̂Ńt���O�����Z�b�g
		m_lotteryChanceFlag = false;
	}
}

bool SummonerSM_Attack::IsNextActionDarkMeteo()
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
	//�_�[�N���e�I�������Ȃ�
	return false;
}
