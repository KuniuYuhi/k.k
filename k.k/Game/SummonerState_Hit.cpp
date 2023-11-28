#include "stdafx.h"
#include "SummonerState_Hit.h"
#include "Summoner.h"

void SummonerState_Hit::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_Hit::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_CriticalHit);
}
