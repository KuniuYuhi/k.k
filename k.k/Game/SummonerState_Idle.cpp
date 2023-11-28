#include "stdafx.h"
#include "SummonerState_Idle.h"
#include "Summoner.h"

void SummonerState_Idle::ManageState()
{
	m_summoner->ProcessCommonStateTransition();
}

void SummonerState_Idle::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Idle, 0.2f);
}
