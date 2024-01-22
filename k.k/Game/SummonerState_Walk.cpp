#include "stdafx.h"
#include "SummonerState_Walk.h"
#include "Summoner.h"

void SummonerState_Walk::ManageState()
{
	//ˆÚ“®ˆ—
	m_summoner->ProcessVigilance();
}

void SummonerState_Walk::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		Summoner::enAnimClip_Walk,0.2f);
}
