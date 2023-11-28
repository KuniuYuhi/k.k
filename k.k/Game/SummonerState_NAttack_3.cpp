#include "stdafx.h"
#include "SummonerState_NAttack_3.h"
#include "Summoner.h"

void SummonerState_NAttack_3::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_NAttack_3::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_3);
}
