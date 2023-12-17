#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"

//アニメーションステート統一してもよいかも

IMobStateMachine* IMobStateMachine::m_stateMachineInstance = nullptr;

IMobStateMachine::IMobStateMachine(MobMonster* mobMonsterInstance)
{
	m_mobMonster = mobMonsterInstance;
}

IMobStateMachine::~IMobStateMachine()
{
	m_stateMachineInstance = nullptr;
}

void IMobStateMachine::Execute()
{
	//行動を決める
	ProcessDecideAction();
}

void IMobStateMachine::ProcessDecideAction()
{
	//一定のアニメーション中は処理しない
	if (m_mobMonster->IsAttackEnable()!=true)
	{
		return;
	}


	//これから行動を決める
	DecideNextAction();
}

void IMobStateMachine::DecideNextAction()
{
	//プレイヤーを見つけたか判定
	if (m_mobMonster->IsFoundPlayer() == true)
	{
		//プレイヤーを見つけたら追いかける
		ProcessChase();
	}
	else
	{
		//プレイヤーを見つけていないならパトロール
		ProcessPatrol();
	}
	
}

void IMobStateMachine::ProcessPatrol()
{

}

void IMobStateMachine::ProcessChase()
{
	//攻撃範囲内にプレイヤーがいるなら
	if (m_mobMonster->IsPlayerInAttackRange() == true)
	{
		//攻撃方法を決める
		ProcessAttackState();
	}
	else
	{
		//追いかける
	}
}

void IMobStateMachine::ProcessAttackState()
{

}

