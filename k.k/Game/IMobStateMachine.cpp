#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"


//				
//
//				�ǂ�������	���\�\�\�\�\�\�\�\�\��	����
//					|								  |
//					|								  |		
//				   �U��								�ҋ@
//					|
//		�ʏ�U���\�\�\�\�\�X�L��				 
// 


IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
}