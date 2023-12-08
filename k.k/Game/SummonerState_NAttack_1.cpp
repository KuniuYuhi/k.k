#include "stdafx.h"
#include "SummonerState_NAttack_1.h"
#include "Summoner.h"

#include "Player.h"
#include "CharactersInfoManager.h"


namespace {
	const float RANGE = 230.0f;

	const int MAX_PROBABILITY = 1;		//�m���̍ő�l

	const int ATTACK_MAX_PROBABILITY = 5;
}


void SummonerState_NAttack_1::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		NextAction();
	}
}

void SummonerState_NAttack_1::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_1);
}

bool SummonerState_NAttack_1::IsPlayerInRange()
{
	//�{�X����v���C���[�Ɍ������x�N�g�����v�Z
	Vector3 diff =
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition() -
		m_summoner->GetPosition();

	//�x�N�g���̒������͈͓��Ȃ�
	if (diff.Length() < RANGE)
	{
		//����
		return true;
	}
	//���Ȃ�
	return false;
}

void SummonerState_NAttack_1::NextAction()
{
	//�������������B
	srand((unsigned)time(NULL));

	//�U����ɂ܂��v���C���[���͈͓��ɂ���Ȃ�
	if (IsPlayerInRange() == true)
	{
		//�U������m�����m��ɂ���
		m_probability = MAX_PROBABILITY;
	}

	//�R���{���邩�ҋ@�ɂ��邩���߂�
	int answer = rand() % m_probability;

	//���ʂɂ���čs���ω�
	if (answer < ATTACK_MAX_PROBABILITY)
	{
		//���̍U���ɑJ��
		m_summoner->SetNextAnimationState(Summoner::enAnimationState_NormalAttack_2);
	}
	else
	{
		//�ҋ@��ԂɑJ��
		m_summoner->ProcessCommonStateTransition();
	}
}
