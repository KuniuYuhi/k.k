#pragma once
#include "ISummonerStateMachine.h"

class Summoner;
class SummonerSMContext;

/// <summary>
/// 基本行動サブステートマシン
/// </summary>
class SummonerSSM_General : public ISummonerStateMachine
{
public:

	SummonerSSM_General(Summoner* summoner, SummonerSMContext* context)
		:ISummonerStateMachine(summoner,context)
	{}


	void Entry() override;

	void Update() override;

	void Exit() override;

private:
	/// <summary>
	/// ステートマシン切り替えタイマーの計算
	/// </summary>
	void CalcStateMachineChangeTimer();

	/// <summary>
	/// 移動パターンを決める
	/// </summary>
	void CheckMovePattern();
	/// <summary>
	/// 移動パターンタイマーの計算
	/// </summary>
	void CalcMovePatternCheckTime();

	/// <summary>
	/// アタックステートマシンい切り替えるかチェックする
	/// </summary>
	void CheckEnableChangeAttackStateMachine();

	/// <summary>
	/// 状況を判断する。状況によってはステートを切り替える
	/// </summary>
	bool EvaluateSituation();


	/// <summary>
	/// ワープステートに切り替えるかチェックする
	/// </summary>
	/// <returns>切り替えるならtrue</returns>
	bool CheckChangeWarpState();


	/// <summary>
	/// 引数のタイマーを加算する
	/// </summary>
	/// <param name="timer"></param>
	void AddTimer(float& timer);

	/// <summary>
	/// プレイヤーが近距離にいる間タイマーを更新する」
	/// </summary>
	void UpdateCloseDistanceTimer();


private:

	float m_stateMachineChangeTimeLimit = 5.0f;
	float m_stateMachineChangeTimer = 0.0f;
	bool m_isEnableChangeStateMachine = false;

	float m_movePatternCheckTimer = 0.0f;
	float m_movePatternCheckTimeLimit = 5.0f;
	bool m_isMovePatternCheck = false;


	float m_warpCoolTimer = 0.0f;
	float m_warpCoolTimeLimit = 18.0f;


	float m_closeDistanceTimer = 0.0f;


	int m_summonerHp = 0;

};

