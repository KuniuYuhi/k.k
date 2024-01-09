#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "Player.h"
#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()の引数にオーダーをいれる

using namespace SummonerActions;

float IBossStateMachine::m_stayPlayerTimer = 0;

namespace {
	const float WAIT_TIME = 500.0f;		//待機時間

	const float MELEE_ATTACK_RANGE = 280.0f;	//近距離攻撃の範囲内

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//プレイヤーが近くにとどまっているタイマーの上限


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

}


IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;

	//乱数を初期化。
	srand((unsigned)time(NULL));
}

IBossStateMachine::~IBossStateMachine()
{
	
}
