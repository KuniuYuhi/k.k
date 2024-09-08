#include "stdafx.h"
#include "SummonerState_Run.h"
#include "Summoner.h"

void SummonerState_Run::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_Run, 0.2f
	);
}

void SummonerState_Run::Entry()
{
	//接近できる距離に到達
	m_summoner->ActiveArrivedApproachDistance();
	//接近できる距離をリセットする
	m_summoner->ResetApproachDistanceValue();
}

void SummonerState_Run::Ubdate()
{
	if (fabsf(m_summoner->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_summoner->GetMoveSpeed().z) <= 0.001f)
	{
		m_summonerStateContext->ChangeState(enSummonerState_Idle);
		return;
	}
}

void SummonerState_Run::Exit()
{
}
