#include "stdafx.h"
#include "SummonerState_Appear.h"
#include "Summoner.h"

void SummonerState_Appear::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Appear, 0.1f
	);
}

void SummonerState_Appear::Entry()
{
	m_summoner->ActionActive();
}

void SummonerState_Appear::Ubdate()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_Appear::Exit()
{
	m_summoner->ActionDeactive();
}
