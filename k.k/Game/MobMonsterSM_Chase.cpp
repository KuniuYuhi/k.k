#include "stdafx.h"
#include "MobMonsterSM_Chase.h"
#include "MobMonster.h"
#include "CharactersInfoManager.h"

#include "GameSceneManager.h"

using namespace MobMonsterInfo;

void MobMonsterSM_Chase::Execute()
{
	if (GameSceneManager::GetInstance()->GetBattleOutCome() !=
		GameSceneManager::enBattleOutCome_None)
	{
		return;
	}

	CharactersInfoManager::GetInstance()->SearchMonsterNearPlayer(m_mobMonster);
	
	//行動を決める
	ProcessDecideAction();
}

void MobMonsterSM_Chase::ProcessDecideAction()
{
	//処理を止めないといけないなら
	if (m_mobMonster->IsStopProcessing() == true)
	{
		return;
	}
	//一定のアニメーション中は処理しない
	if (m_mobMonster->IsAttackEnable() != true)
	{
		return;
	}
	if (m_mobMonster->isAnimationEnable() != true)
	{
		return;
	}
	//行動を決める
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
		//追いかける
		m_mobMonster->SetNextAnimationState(enAninationState_Chase);
	}

}
