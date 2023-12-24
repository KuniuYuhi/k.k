#include "stdafx.h"
#include "SummonerState_DarkSpearEnd.h"
#include "Summoner.h"

void SummonerState_DarkSpearEnd::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_DarkSpearEnd::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimationState_DarkSpear_End, 0.1f);
}
