#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_DarkBall, 0.2f
	);
}

void SummonerState_DarkBall::Entry()
{
	//�A�N�V������
	m_summoner->ActionActive();
}

void SummonerState_DarkBall::Ubdate()
{
	//��]��������
	m_summoner->UpdateAttackTurnToTargetProcess();

	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		m_summoner->CheckAndTransitionStateMachine();
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}

}

void SummonerState_DarkBall::Exit()
{
	//�A�N�V�����I���
	m_summoner->ActionDeactive();
}
