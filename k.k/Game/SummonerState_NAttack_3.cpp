#include "stdafx.h"
#include "SummonerState_NAttack_3.h"
#include "Summoner.h"

void SummonerState_NAttack_3::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//攻撃ステートが終わった後の処理
		m_summoner->ProcessEndAttackState();

	}
}

void SummonerState_NAttack_3::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_NormalAttack_3);
}
