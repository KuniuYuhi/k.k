#include "stdafx.h"
#include "SummonerState_DMeteo_Main.h"
#include "Summoner.h"

void SummonerState_DMeteo_Main::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_DMeteo_Main::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Attack_DarkMeteorite_main);
}
