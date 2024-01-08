#include "stdafx.h"
#include "MobMonsterSM_Chase.h"
#include "MobMonster.h"
#include "CharactersInfoManager.h"

using namespace MobMonsterInfo;

void MobMonsterSM_Chase::Execute()
{
	CharactersInfoManager::GetInstance()->SearchMonsterNearPlayer(m_mobMonster);
	//行動を決める
	ProcessDecideAction();
}

void MobMonsterSM_Chase::ProcessDecideAction()
{
	//一定のアニメーション中は処理しない
	if (m_mobMonster->IsAttackEnable() != true)
	{
		return;
	}
	if (m_mobMonster->isAnimationEnable() != true)
	{
		return;
	}

	DecideNextAction();
}

void MobMonsterSM_Chase::DecideNextAction()
{
	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//プレイヤーを見つけたら追いかける
		ProcessChase();
	}
	else
	{
		//プレイヤーを見失ったらステートマシンを切り替える
		m_mobMonster->SetNextStateMachine(enStateMachineState_Patrol);
	}

}

void MobMonsterSM_Chase::ProcessAttackState()
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

void MobMonsterSM_Chase::ProcessChase()
{


	//攻撃範囲内にプレイヤーがいるなら
	if (m_mobMonster->IsPlayerInAttackRange() == true)
	{
		//攻撃方法を決める
		ProcessAttackState();
	}
	else
	{
		
		//if (CharactersInfoManager::GetInstance()->
		//	SearchMonsterNearPlayer(m_mobMonster)
		//	== true
		//	)
		//{
		//	//十分近づいているので待機
		//	m_mobMonster->SetNextAnimationState(enAninationState_Idle);
		//	return;
		//}

		

		//追いかける
		m_mobMonster->SetNextAnimationState(enAninationState_Chase);
	}

}
