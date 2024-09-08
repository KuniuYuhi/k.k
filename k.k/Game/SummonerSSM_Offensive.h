#pragma once
#include "ISummonerStateMachine.h"
#include "SummonerInfo.h"

class Summoner;
class SummonerSMContext;

class Brave;

using namespace SummonerStates;

/// <summary>
/// 攻撃サブステートマシン
/// </summary>
class SummonerSSM_Offensive : public ISummonerStateMachine
{
public:

	SummonerSSM_Offensive(Summoner* summoner, SummonerSMContext* context)
		:ISummonerStateMachine(summoner, context)
	{
	}


	void Entry() override;

	void Update() override;

	//virtual void Exit() {}

	bool IsEnableChangeStateMachine() override;

private:

	/// <summary>
	/// 距離によって遠距離か近距離攻撃か選ぶ
	/// </summary>
	void SelectAttackBasedOnDistance();

	/// <summary>
	/// 近距離攻撃の中から選ぶ
	/// </summary>
	void SelectMeleeAttack();
	/// <summary>
	/// 遠距離攻撃の中から選ぶ
	/// </summary>
	void SelectRangedAttack();


	/// <summary>
	/// 次のコンボ攻撃にする
	/// </summary>
	bool ChangeNextComboAttackState();



	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CheckChangeDarkBallState();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CheckChangeDarkMeteoriteState();



private:

	Brave* m_player = nullptr;


	const int m_MaxUseDarkMeteoriteCount = 4;
	int m_darkMeteoriteRemainingCount = m_MaxUseDarkMeteoriteCount;		//残り回数

	float m_darkMeteoriteCoolTimer = 0.0f;
};

