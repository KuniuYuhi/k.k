#include "stdafx.h"
#include "SummonerState_KnockBack.h"
#include "Summoner.h"

void SummonerState_KnockBack::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_KnockBack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_KnockBack);
}
