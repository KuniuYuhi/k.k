#include "stdafx.h"
#include "SummonerState_Command.h"
#include "Summoner.h"

void SummonerState_Command::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_Command::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Command);
}
