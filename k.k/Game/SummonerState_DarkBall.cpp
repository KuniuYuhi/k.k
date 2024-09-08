#include "stdafx.h"
#include "SummonerState_DarkBall.h"
#include "Summoner.h"

void SummonerState_DarkBall::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_DarkBall, 0.2f
	);
}

void SummonerState_DarkBall::Entry()
{
	//アクション中
	m_summoner->ActionActive();
}

void SummonerState_DarkBall::Ubdate()
{
	//回転だけする
	m_summoner->UpdateAttackTurnToTargetProcess();

	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートマシンを切り替えるのかチェック
		m_summoner->CheckAndTransitionStateMachine();
		//共通ステート
		m_summoner->ProcessCommonTranstion();
	}

}

void SummonerState_DarkBall::Exit()
{
	//アクション終わり
	m_summoner->ActionDeactive();
}
