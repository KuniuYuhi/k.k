#include "stdafx.h"
#include "BeholderEyeStateContext.h"
#include "IMobEnemyState.h"

#include "BeholderEyeState_Idle.h"
#include "BeholderEyeState_Run.h"
#include "BeholderEyeState_Attack.h"
#include "BeholderEyeState_Hit.h"
#include "BeholderEyeState_Die.h"
#include "BeholderEyeState_Victory.h"
#include "BeholderEyeState_Appear.h"



BeholderEyeStateContext::BeholderEyeStateContext()
{
}

BeholderEyeStateContext::~BeholderEyeStateContext()
{
	m_currentState = nullptr;
}

void BeholderEyeStateContext::Init(BeholderEye* beholderEye, EnBeholderEyeState startState)
{
	m_enCurrentSlimeState = startState;
	//ステートを切り替える
	ChangeBeholderEyeState(beholderEye, startState);
}

void BeholderEyeStateContext::ChangeBeholderEyeState(BeholderEye* beholderEye, EnBeholderEyeState changeState)
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
	case BeholderEyeStates::enBeholderEyeState_Idle:
		m_currentState = new BeholderEyeState_Idle(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Run:
		m_currentState = new BeholderEyeState_Run(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Attack:
		m_currentState = new BeholderEyeState_Attack(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Hit:
		m_currentState = new BeholderEyeState_Hit(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Die:
		m_currentState = new BeholderEyeState_Die(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Victory:
		m_currentState = new BeholderEyeState_Victory(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Appear:
		m_currentState = new BeholderEyeState_Appear(beholderEye, this);
		break;
	default:
		break;
	}

	//最初の処理を実行
	m_currentState->Entry();

	//現在のステートの値を保存
	m_enCurrentSlimeState = changeState;
}

void BeholderEyeStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;
	//更新処理
	m_currentState->Ubdate();
}

void BeholderEyeStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;
	//アニメーションを再生
	m_currentState->PlayAnimation();
}
