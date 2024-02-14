#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"


IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
}