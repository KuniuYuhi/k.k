#include "stdafx.h"
#include "IMobStateMachine.h"
#include "MobMonster.h"

//アニメーションステート統一してもよいかも
using namespace MobMonsterInfo;

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
	if (m_mobMonster->isAnimationEnable() != true)
	{
		return;
	}


	//これから行動を決める
	DecideNextAction();
}

void IMobStateMachine::DecideNextAction()
{
	//プレイヤーを見つけたか判定

	//todo 既にプレイヤーの周りに敵が複数体いるなら近よらない

	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//プレイヤーを見つけたら追いかける
		ProcessChase();
	}
	else
	{
		//プレイヤーを見つけていないならパトロールor待機
		ProcessPatrol();
	}

}

void IMobStateMachine::ProcessPatrol()
{
	//巡回する
	m_mobMonster->SetNextAnimationState(enAninationState_Patrol);
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
		m_mobMonster->SetNextAnimationState(enAninationState_Chase);
	}
}

void IMobStateMachine::ProcessAttackState()
{
	//まず攻撃ができるか
	if (m_mobMonster->IsProcessAttackEnable() == true)
	{
		//スキル攻撃ができる状態なら
		if (m_mobMonster->IsSkillUsable() == true)
		{
			//スキル攻撃
			m_mobMonster->SetNextAnimationState(enAnimationState_Skill);
		}
		else
		{
			//通常攻撃
			m_mobMonster->SetNextAnimationState(enAnimationState_Attack);
		}
	}
	else
	{
		//攻撃できないならその場で待機
		m_mobMonster->SetNextAnimationState(enAninationState_Idle);
	}
}

	

