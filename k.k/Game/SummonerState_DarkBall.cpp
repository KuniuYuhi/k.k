#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

#include "DarkBall.h"

//todo �T���i�[�̒��g�����������Ȃ�
namespace {
	const int CONTINUOUS_ATTACK_COUNT_LIMMIT = 2;
	const int ONE_MORE_ATTACK_PROBABILITY = 5;
}

SummonerState_DarkBall::~SummonerState_DarkBall()
{

}

void SummonerState_DarkBall::ManageState()
{
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		

		//�A���U���񐔂�����ȉ��Ȃ�
		if (m_summoner->GetNowStateMachine()->GetContinuousAttackCount() <
			CONTINUOUS_ATTACK_COUNT_LIMMIT)
		{

			//������x�U������m�������߂�
			int num = rand() % 10;
			//������x�U������m������������U������
			if (num > ONE_MORE_ATTACK_PROBABILITY)
			{
				//�U���܂őҋ@
				m_summoner->ProcessCommonStateTransition();
				return;
			}
		}
		//�U���X�e�[�g���I�������̏����Btodo �����őS������������
		m_summoner->ProcessEndAttackState();

	}
}

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkBall);
}
