#include "stdafx.h"
#include "CactusStateContext.h"
#include "IMobEnemyState.h"

#include "CactusState_Idle.h"
#include "CactusState_Run.h"
#include "CactusState_Attack.h"
#include "CactusState_Hit.h"
#include "CactusState_Die.h"
#include "CactusState_Victory.h"
#include "CactusState_Appear.h"

CactusStateContext::CactusStateContext()
{
}

CactusStateContext::~CactusStateContext()
{
	m_currentState = nullptr;
}

void CactusStateContext::Init(Cactus* cactus, EnCactusState startState)
{
	m_enCurrentSlimeState = startState;
	//ステートを切り替える
	ChangeCactusState(cactus, startState);
}

void CactusStateContext::ChangeCactusState(Cactus* cactus, EnCactusState changeState)
{
	//前のステートが何だったか保存しておく
	m_enPreviousSlimeState = m_enCurrentSlimeState;

	if (m_currentState != nullptr)
	{
		//現在のステートを抜け出す時の処理
		m_currentState->Exit();
	}

	m_currentState = nullptr;


	//新しいステートクラスを代入
	switch (changeState)
	{
	case CactusStates::enCactusState_Idle:
		m_currentState = new CactusState_Idle(cactus, this);
		break;
	case CactusStates::enCactusState_Run:
		m_currentState = new CactusState_Run(cactus, this);
		break;
	case CactusStates::enCactusState_Attack:
		m_currentState = new CactusState_Attack(cactus, this);
		break;
	case CactusStates::enCactusState_Hit:
		m_currentState = new CactusState_Hit(cactus, this);
		break;
	case CactusStates::enCactusState_Die:
		m_currentState = new CactusState_Die(cactus, this);
		break;
	case CactusStates::enCactusState_Victory:
		m_currentState = new CactusState_Victory(cactus, this);
		break;
	case CactusStates::enCactusState_Appear:
		m_currentState = new CactusState_Appear(cactus, this);
		break;
	default:
		break;
	}

	//最初の処理を実行
	m_currentState->Entry();

	//現在のステートの値を保存
	m_enCurrentSlimeState = changeState;
}

void CactusStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;
	//更新処理
	m_currentState->Ubdate();
}

void CactusStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;
	//アニメーションを再生
	m_currentState->PlayAnimation();
}
