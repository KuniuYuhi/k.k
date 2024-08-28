#include "stdafx.h"
#include "SlimeStateContext.h"
#include "IMobEnemyState.h"

#include "SlimeState_Idle.h"
#include "SlimeState_Run.h"
#include "SlimeState_Attack.h"
#include "SlimeState_Hit.h"
#include "SlimeState_Die.h"
#include "SlimeState_Victory.h"
#include "SlimeState_Appear.h"



SlimeStateContext::SlimeStateContext()
{
}

SlimeStateContext::~SlimeStateContext()
{
	m_currentSlimeState = nullptr;
}

void SlimeStateContext::Init(Slime* slime, EnSlimeState startState)
{
	m_enCurrentSlimeState = startState;
	//ステートを切り替える
	ChangeSlimeState(slime, startState);
}

void SlimeStateContext::ChangeSlimeState(Slime* slime, EnSlimeState changeState)
{
	//前のステートが何だったか保存しておく
	m_enPreviousSlimeState = m_enCurrentSlimeState;

	if (m_currentSlimeState != nullptr)
	{
		//現在のステートを抜け出す時の処理
		m_currentSlimeState->Exit();
	}

	m_currentSlimeState = nullptr;

	//新しいステートクラスを代入
	switch (changeState)
	{
	case SlimeStates::enSlimeState_Idle:
		m_currentSlimeState = new SlimeState_Idle(slime, this);
		break;
	case SlimeStates::enSlimeState_Run:
		m_currentSlimeState = new SlimeState_Run(slime, this);
		break;
	case SlimeStates::enSlimeState_Attack:
		m_currentSlimeState = new SlimeState_Attack(slime, this);
		break;
	case SlimeStates::enSlimeState_Hit:
		m_currentSlimeState = new SlimeState_Hit(slime, this);
		break;
	case SlimeStates::enSlimeState_Die:
		m_currentSlimeState = new SlimeState_Die(slime, this);
		break;
	case SlimeStates::enSlimeState_Victory:
		m_currentSlimeState = new SlimeState_Victory(slime, this);
		break;
	case SlimeStates::enSlimeState_Appear:
		m_currentSlimeState = new SlimeState_Appear(slime, this);
		break;
	default:
		break;
	}

	//最初の処理を実行
	m_currentSlimeState->Entry();

	//現在のステートの値を保存
	m_enCurrentSlimeState = changeState;

}

void SlimeStateContext::UpdateCurrentState()
{
	if (m_currentSlimeState == nullptr) return;
	//更新処理
	m_currentSlimeState->Ubdate();
}

void SlimeStateContext::PlayAnimationCurrentState()
{
	if (m_currentSlimeState == nullptr) return;
	//アニメーションを再生
	m_currentSlimeState->PlayAnimation();
}
