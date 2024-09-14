#include "stdafx.h"
#include "SummonerStateContext.h"

#include "ISummonerState.h"

#include "SummonerState_Appear.h"
#include "SummonerState_Idle.h"
#include "SummonerState_Run.h"
#include "SummonerState_DarkBall.h"
#include "SummonerState_DarkMeteorite.h"
#include "SummonerState_FirstAttack.h"
#include "SummonerState_SecondAttack.h"
#include "SummonerState_ThirdAttack.h"
#include "SummonerState_ShockWave.h"
#include "SummonerState_Hit.h"
#include "SummonerState_Die.h"
#include "SummonerState_Victory.h"
#include "SummonerState_Warp.h"


SummonerStateContext::SummonerStateContext()
{
}

SummonerStateContext::~SummonerStateContext()
{
	m_summonerStateMap.clear();
	m_currentState = nullptr;
}

void SummonerStateContext::Init(Summoner* summoner, EnSummonerState startState)
{
	
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Appear, new SummonerState_Appear(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Idle, new SummonerState_Idle(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Run, new SummonerState_Run(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_DarkBall, new SummonerState_DarkBall(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Attack_DarkMeteorite, new SummonerState_DarkMeteorite(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_FirstAttack, new SummonerState_FirstAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_SecondAttack, new SummonerState_SecondAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_ThirdAttack, new SummonerState_ThirdAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_ShockWave, new SummonerState_ShockWave(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Hit, new SummonerState_Hit(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Die, new SummonerState_Die(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Victory, new SummonerState_Victory(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Warp, new SummonerState_Warp(summoner, this)));

	ChangeState(startState);
}

void SummonerStateContext::ChangeState(EnSummonerState changeState)
{
	//前のステートを保存しておく
	m_EnPreviousState = m_EnCurrentState;

	if (m_currentState != nullptr)
	{
		//現在のステートの最後の処理を行う
		m_currentState->Exit();
	}

	//nullにする
	m_currentState = nullptr;

	//マップからキーに対応するステートを持ってくる
	//新しいステートに入れ替える
	m_currentState = m_summonerStateMap.at(changeState);
	//キーが対応してなかったら
	if (m_currentState == nullptr)
	{
		std::abort();
		return;
	}

	//最初の処理を実行
	m_currentState->Entry();

	//現在のステートの値を保存
	m_EnCurrentState = changeState;

}

void SummonerStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;

	m_currentState->Ubdate();
}

void SummonerStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;

	m_currentState->PlayAnimation();
}
