#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "Player.h"
#include "CharactersInfoManager.h"

using namespace SummonerActions;

float IBossStateMachine::m_stayPlayerTimer = 0;

IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;

	//乱数を初期化。
	srand((unsigned)time(NULL));
}

IBossStateMachine::~IBossStateMachine()
{
	
}
