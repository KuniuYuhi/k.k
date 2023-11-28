#include "stdafx.h"
#include "SummonerState_DarkWall.h"
#include "Summoner.h"

void SummonerState_DarkWall::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_DarkWall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkWall);
}
