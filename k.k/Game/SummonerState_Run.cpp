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
	//�ڋ߂ł��鋗���ɓ��B
	m_summoner->ActiveArrivedApproachDistance();
	//�ڋ߂ł��鋗�������Z�b�g����
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
