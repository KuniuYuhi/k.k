#include "stdafx.h"
#include "SummonerState_DMeteo_Start.h"
#include "Summoner.h"

void SummonerState_DMeteo_Start::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->
			SetNextAnimationState(Summoner::enAnimationState_Attack_DarkMeteorite_main);
	}
}

void SummonerState_DMeteo_Start::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Attack_DarkMeteorite_start);
}
