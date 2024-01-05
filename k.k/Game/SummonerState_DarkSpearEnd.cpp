#include "stdafx.h"
#include "SummonerState_DarkSpearEnd.h"
#include "Summoner.h"

void SummonerState_DarkSpearEnd::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�U���X�e�[�g���I�������̏���
		m_summoner->ProcessEndAttackState();

	}
}

void SummonerState_DarkSpearEnd::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimationState_DarkSpear_End, 0.1f);
}
