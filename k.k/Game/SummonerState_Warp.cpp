#include "stdafx.h"
#include "SummonerState_Warp.h"
#include "Summoner.h"

void SummonerState_Warp::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Warp
	);
}

void SummonerState_Warp::Entry()
{
	m_summoner->ActionActive();
	//���G�ɂ���
	m_summoner->InvicibleActive();
}

void SummonerState_Warp::Ubdate()
{


	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_Warp::Exit()
{
	m_summoner->ActionDeactive();
	//���G������
	m_summoner->InvicibleDeactive();
}
