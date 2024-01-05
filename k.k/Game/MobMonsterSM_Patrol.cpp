#include "stdafx.h"
#include "MobMonsterSM_Patrol.h"
#include "MobMonster.h"

using namespace MobMonsterInfo;

void MobMonsterSM_Patrol::Execute()
{
	//行動を決める
	ProcessDecideAction();
}

void MobMonsterSM_Patrol::ProcessDecideAction()
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

	if (m_mobMonster->IsFoundPlayerFlag() == true)
	{
		//プレイヤーを見つけたら追いかけるのでステートマシンを切り替える
		m_mobMonster->SetNextStateMachine(enStateMachineState_Chase);
	}
	else
	{
		//プレイヤーを見つけていないならパトロールor待機
		ProcessPatrol();
	}
}

void MobMonsterSM_Patrol::ProcessPatrol()
{


	//巡回する
	m_mobMonster->SetNextAnimationState(enAninationState_Patrol);
}
