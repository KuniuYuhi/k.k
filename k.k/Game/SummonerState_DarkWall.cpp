#include "stdafx.h"
#include "SummonerState_DarkWall.h"
#include "Summoner.h"

void SummonerState_DarkWall::ManageState()
{
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		
		//攻撃ステートが終わった後の処理
		m_summoner->ProcessEndAttackState();
	}
}

void SummonerState_DarkWall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkWall);
}
