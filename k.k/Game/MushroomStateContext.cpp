#include "stdafx.h"
#include "MushroomStateContext.h"
#include "IMobEnemyState.h"

#include "MushroomState_Idle.h"
#include "MushroomState_Run.h"
#include "MushroomState_Attack.h"
#include "MushroomState_Hit.h"
#include "MushroomState_Die.h"
#include "MushroomState_Victory.h"
#include "MushroomState_Appear.h"


MushroomStateContext::MushroomStateContext()
{
}

MushroomStateContext::~MushroomStateContext()
{
	m_currentState = nullptr;
}

void MushroomStateContext::Init(Mushroom* mushroom, EnMushroomState startState)
{
	m_enCurrentSlimeState = startState;
	//ステートを切り替える
	ChangeMushroomState(mushroom, startState);
}

void MushroomStateContext::ChangeMushroomState(Mushroom* mushroom, EnMushroomState changeState)
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
	case MushroomStates::enMushroomState_Idle:
		m_currentState = new MushroomState_Idle(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Run:
		m_currentState = new MushroomState_Run(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Attack:
		m_currentState = new MushroomState_Attack(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Hit:
		m_currentState = new MushroomState_Hit(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Die:
		m_currentState = new MushroomState_Die(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Victory:
		m_currentState = new MushroomState_Victory(mushroom, this);
		break;
	case MushroomStates::enMushroomState_Appear:
		m_currentState = new MushroomState_Appear(mushroom, this);
		break;
	default:
		break;
	}

	//最初の処理を実行
	m_currentState->Entry();

	//現在のステートの値を保存
	m_enCurrentSlimeState = changeState;
}

void MushroomStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;
	//更新処理
	m_currentState->Ubdate();
}

void MushroomStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;
	//アニメーションを再生
	m_currentState->PlayAnimation();
}
