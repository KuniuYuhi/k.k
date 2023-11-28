#include "stdafx.h"
#include "SummonerState_NAttack_2.h"
#include "Summoner.h"

void SummonerState_NAttack_2::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->SetNextAnimationState(Summoner::enAnimationState_NormalAttack_3);
		//m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_NAttack_2::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_2);
}
