#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"


//				
//
//				追いかける	＜―――――――――＞	巡回
//					|								  |
//					|								  |		
//				   攻撃								待機
//					|
//		通常攻撃―――――スキル				 
// 


IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
}