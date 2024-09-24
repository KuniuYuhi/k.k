#include "stdafx.h"
#include "SummonerState_DarkMeteorite.h"
#include "Summoner.h"

void SummonerState_DarkMeteorite::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Attack_DarkMeteorite_start, 0.1f
	);
}

void SummonerState_DarkMeteorite::Entry()
{
	m_summoner->ActionActive();
	//���G�ɂ���
	m_summoner->InvicibleActive();
}

void SummonerState_DarkMeteorite::Ubdate()
{

	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		m_summoner->CheckAndTransitionStateMachine();
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_DarkMeteorite::Exit()
{
	m_summoner->ActionDeactive();
	//���G������
	m_summoner->InvicibleDeactive();
}
