#include "stdafx.h"
#include "SummonerState_DarkMeteorite.h"
#include "Summoner.h"

void SummonerState_DarkMeteorite::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Attack_DarkMeteorite_start, 0.1f
	);
}

void SummonerState_DarkMeteorite::Entry()
{
	m_summoner->ActionActive();
	//無敵にする
	m_summoner->InvicibleActive();
}

void SummonerState_DarkMeteorite::Ubdate()
{

	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		m_summoner->CheckAndTransitionStateMachine();
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_DarkMeteorite::Exit()
{
	m_summoner->ActionDeactive();
	//無敵を解除
	m_summoner->InvicibleDeactive();
}
