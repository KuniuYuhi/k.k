#include "stdafx.h"
#include "SummonerState_Summon.h"
#include "Summoner.h"

void SummonerState_Summon::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_Summon::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Summon);
}
