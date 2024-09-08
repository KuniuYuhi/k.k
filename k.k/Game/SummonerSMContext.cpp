#include "stdafx.h"
#include "SummonerSMContext.h"

#include "ISummonerStateMachine.h"

#include "SummonerSSM_General.h"
#include "SummonerSSM_Offensive.h"

#include "Summoner.h"


namespace {
	const int QUOTIENT_VALUE = 4;
}

SummonerSMContext::SummonerSMContext()
{
}

SummonerSMContext::~SummonerSMContext()
{
}

void SummonerSMContext::Init(
	Summoner* summoner, EnSubStateMachine startSubStateMchine)
{
	//最大HPを割った商を計算
	CalcMaxHpQuotient(summoner->GetCommonStatus().GetMaxHp());

	//ステートマシンをマップに追加する
	m_subStateMachineMap.insert(std::make_pair(enSubStateMachine_General, new SummonerSSM_General(summoner,this)));
	m_subStateMachineMap.insert(std::make_pair(enSubStateMachine_Offensive, new SummonerSSM_Offensive(summoner,this)));

	//ステートマシンを切り替える
	ChangeSubStateMachine(startSubStateMchine);
}

void SummonerSMContext::ChangeSubStateMachine(EnSubStateMachine changeSubStateMachine)
{
	if (m_currentStateMachine != nullptr)
	{
		m_currentStateMachine->Exit();
	}

	m_currentStateMachine = nullptr;

	//マップからキーを使って対応するサブステートマシンを持ってくる
	m_currentStateMachine = m_subStateMachineMap.at(changeSubStateMachine);

	if (m_currentStateMachine == nullptr) std::abort();

	//現在のサブステートマシンの最初の処理を実行
	m_currentStateMachine->Entry();

	m_enCurrentSubStateMachine = changeSubStateMachine;
}

void SummonerSMContext::CurrentSubStateMachineUpdate()
{
	if (m_currentStateMachine == nullptr) return;

	//サブステートマシンの更新処理
	m_currentStateMachine->Update();
}

bool SummonerSMContext::CheckAndTransitionStateMachine()
{
	return m_currentStateMachine->IsEnableChangeStateMachine();
}

void SummonerSMContext::CalcMaxHpQuotient(int maxHp)
{
	m_maxHpQuotient = maxHp / QUOTIENT_VALUE;
}
