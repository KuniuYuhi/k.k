#include "stdafx.h"
#include "IBossStateMachine.h"
#include "Summoner.h"
#include "ISummonerState.h"

#include "Player.h"
#include "CharactersInfoManager.h"


//todo ProcessMeleeAttack(),ProcessLongRangeAttack()�̈����ɃI�[�_�[�������

using namespace SummonerActions;

float IBossStateMachine::m_stayPlayerTimer = 0;

namespace {
	const float WAIT_TIME = 500.0f;		//�ҋ@����

	const float MELEE_ATTACK_RANGE = 280.0f;	//�ߋ����U���͈͓̔�

	const float STAY_PLAYER_LIMMIT_TIME = 10.0f;		//�v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�̏��


	const float KNOCKBACK_DISTANCE = 200.0f;

	const float DARK_METEO_ACTION_POINT = 40.0f;

}


IBossStateMachine::IBossStateMachine(Summoner* bossInstance)
{
	m_summoner = bossInstance;

	//�������������B
	srand((unsigned)time(NULL));
}

IBossStateMachine::~IBossStateMachine()
{
	
}
