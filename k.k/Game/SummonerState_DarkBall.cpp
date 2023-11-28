#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

void SummonerState_DarkBall::ManageState()
{
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		
		m_summoner->ProcessCommonStateTransition();
	}
}

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkBall);
}
