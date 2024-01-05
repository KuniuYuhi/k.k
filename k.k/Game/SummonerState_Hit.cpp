#include "stdafx.h"
#include "SummonerState_Hit.h"
#include "Summoner.h"

void SummonerState_Hit::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�U���X�e�[�g���I�������̏���
		m_summoner->ProcessEndAttackState();
	}
}

void SummonerState_Hit::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_CriticalHit);
}
