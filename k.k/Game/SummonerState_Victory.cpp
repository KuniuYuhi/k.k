#include "stdafx.h"
#include "SummonerState_Victory.h"
#include "Summoner.h"

void SummonerState_Victory::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Victory, 0.1f
	);
}

void SummonerState_Victory::Entry()
{
	m_summoner->ActionActive();
}

void SummonerState_Victory::Ubdate()
{
}

void SummonerState_Victory::Exit()
{
}
