#include "stdafx.h"
#include "SummonerState_NAttack_1.h"
#include "Summoner.h"

void SummonerState_NAttack_1::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		m_summoner->SetNextAnimationState(Summoner::enAnimationState_NormalAttack_2);
		//m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_NAttack_1::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_1);
}
