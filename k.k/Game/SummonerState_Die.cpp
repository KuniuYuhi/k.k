#include "stdafx.h"
#include "SummonerState_Die.h"
#include "Summoner.h"

void SummonerState_Die::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//やーらーれーたー
	}
}

void SummonerState_Die::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Die);
}
