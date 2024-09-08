#pragma once
#include "SummonerInfo.h"

class Summoner;
class SummonerSMContext;

/// <summary>
/// サモナーのステートマシンの基底クラス
/// </summary>
class ISummonerStateMachine
{
public:
	ISummonerStateMachine(Summoner* summoner, SummonerSMContext* context)
	{
		m_summoner = summoner;
		m_stateMachineContext = context;
	}


	~ISummonerStateMachine() {}


	virtual void Entry(){}

	virtual void Update() = 0;

	virtual void Exit() {}

	virtual bool IsEnableChangeStateMachine() { return false; }

protected:


	Summoner* m_summoner = nullptr;
	SummonerSMContext* m_stateMachineContext = nullptr;


};

