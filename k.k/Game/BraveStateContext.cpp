#include "stdafx.h"
#include "BraveStateContext.h"


#include "BraveAllStateFiles.h"



BraveStateContext::BraveStateContext()
{
}

BraveStateContext::~BraveStateContext()
{
	m_braveStateMap.clear();
}

void BraveStateContext::Init(Brave* brave, EnBraveState startState)
{
	//IBraveState* setState = new BraveState_Idle(brave);
	//ステートをマップに格納する
	m_braveStateMap.insert(std::make_pair(enBraveState_Idle, new BraveState_Idle(brave,this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Sprint, new BraveState_Sprint(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_FirstAttack, new BraveState_FirstAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SecondAttack, new BraveState_SecondAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_ThirdAttack, new BraveState_ThirdAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SkillStart, new BraveState_SkillStart(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SkillMain, new BraveState_SkillMain(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Hit, new BraveState_Hit(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_DefensiveActions, new BraveState_DefensiveAction(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_DefensiveHit, new BraveState_DefensiveHit(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Die, new BraveState_Die(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_ChangeWeapon, new BraveState_ChangeWeapon(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_WinStart, new BraveState_WinStart(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_WinMain, new BraveState_WinMain(brave, this)));


	//ステートを切り替える
	ChangeBraveState(startState);

}

void BraveStateContext::ChangeBraveState(EnBraveState changeState)
{
	//前のステートを保存しておく
	m_EnPreviousBraveState = m_EnCurrentBraveState;

	if (m_currentBraveState != nullptr)
	{
		//現在のステートの最後の処理を行う
		m_currentBraveState->Exit();
	}

	//nullにする
	m_currentBraveState = nullptr;

	//マップからキーに対応するステートを持ってくる
	//新しいステートに入れ替える
	m_currentBraveState = m_braveStateMap.at(changeState);
	//キーが対応してなかったら
	if (m_currentBraveState == nullptr)
	{
		std::abort();
		return;
	}

	//最初の処理を実行
	m_currentBraveState->Entry();

	//現在のステートの値を保存
	m_EnCurrentBraveState = changeState;

}

void BraveStateContext::ChangeBraveState(int changeStateNum)
{
	EnBraveState tempState = (EnBraveState)changeStateNum;


	//前のステートを保存しておく
	m_EnPreviousBraveState = m_EnCurrentBraveState;

	if (m_currentBraveState != nullptr)
	{
		//現在のステートの最後の処理を行う
		m_currentBraveState->Exit();
	}

	//nullにする
	m_currentBraveState = nullptr;

	//マップからキーに対応するステートを持ってくる
	//新しいステートに入れ替える
	m_currentBraveState = m_braveStateMap.at(tempState);
	//キーが対応してなかったら
	if (m_currentBraveState == nullptr)
	{
		std::abort();
		return;
	}

	//最初の処理を実行
	m_currentBraveState->Entry();

	//現在のステートの値を保存
	m_EnCurrentBraveState = tempState;
}

void BraveStateContext::UpdateCurrentState()
{
	if (m_currentBraveState == nullptr)
	{
		return;
	}
	//現在のステートの更新処理
	m_currentBraveState->Ubdate();

}

void BraveStateContext::PlayAnimationCurrentState()
{
	if (m_currentBraveState == nullptr)
	{
		return;
	}
	//アニメーションを再生
	m_currentBraveState->PlayAnimation();
}
