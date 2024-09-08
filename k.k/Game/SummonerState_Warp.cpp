#include "stdafx.h"
#include "SummonerState_Warp.h"
#include "Summoner.h"

void SummonerState_Warp::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Warp
	);
}

void SummonerState_Warp::Entry()
{
	m_summoner->ActionActive();
	//無敵にする
	m_summoner->InvicibleActive();
}

void SummonerState_Warp::Ubdate()
{


	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_Warp::Exit()
{
	m_summoner->ActionDeactive();
	//無敵を解除
	m_summoner->InvicibleDeactive();
}
