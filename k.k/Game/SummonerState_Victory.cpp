#include "stdafx.h"
#include "SummonerState_Victory.h"
#include "Summoner.h"

void SummonerState_Victory::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�����X�^�[�̏����I
	}
}

void SummonerState_Victory::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Victory);
}
