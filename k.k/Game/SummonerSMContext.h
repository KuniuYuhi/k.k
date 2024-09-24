#pragma once

#include "SummonerInfo.h"

class Summoner;
class ISummonerStateMachine;
class SummonerStateContext; 

using namespace SummonerStates;
using namespace SummonerSubStateMachine;

/// <summary>
/// サモナーのサブステートマシン管理クラス
/// </summary>
class SummonerSMContext
{
public:

	SummonerSMContext();
	~SummonerSMContext();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="summoner">サモナーのインスタンス</param>
	/// <param name="startSubStateMchine">最初のサブステートマシン</param>
	void Init(
		Summoner* summoner,
		EnSubStateMachine startSubStateMchine
	);

	/// <summary>
	/// サブステートマシンを切り替える
	/// </summary>
	/// <param name="changeSubStateMachine"></param>
	void ChangeSubStateMachine(EnSubStateMachine changeSubStateMachine);


	/// <summary>
	/// 現在のサブステートマシンの更新処理を実行
	/// </summary>
	void CurrentSubStateMachineUpdate();


	/// <summary>
	/// 
	/// </summary>
	bool CheckAndTransitionStateMachine();


	/// <summary>
	/// 予約ステートを取得
	/// </summary>
	/// <returns></returns>
	EnSummonerState GetReservationState()
	{
		return m_reservationState;
	}
	/// <summary>
	/// 予約ステートを設定
	/// </summary>
	/// <param name="reservationState"></param>
	void SetReservationState(EnSummonerState reservationState)
	{
		m_reservationState = reservationState;
	}
	/// <summary>
	/// 予約ステートはあるか？
	/// </summary>
	/// <returns>あるならtrue</returns>
	bool IsReservationState()
	{
		return m_reservationState != enSummonerState_None;
	}

	/// <summary>
	/// 前回のステートを取得
	/// </summary>
	/// <returns></returns>
	EnSummonerState GetPreviousState()
	{
		return m_previousState;
	}
	/// <summary>
	/// 前回のステートを設定
	/// </summary>
	/// <param name="reservationState"></param>
	void SetPreviousState(EnSummonerState reservationState)
	{
		m_previousState = reservationState;
	}


	const int GetMaxHpQuotient()
	{
		return m_maxHpQuotient;
	}

private:

	/// <summary>
	/// 
	/// </summary>
	void CalcMaxHpQuotient(int maxHp);

private:


	//サブステートマシンマップ
	std::map<EnSubStateMachine, ISummonerStateMachine*> m_subStateMachineMap;

	ISummonerStateMachine* m_currentStateMachine = nullptr;		//現在のサブステートマシン

	EnSubStateMachine m_enCurrentSubStateMachine;

	//ステートコンテキスト

	EnSummonerState m_reservationState = enSummonerState_None;		//予約ステート

	EnSummonerState m_previousState = enSummonerState_None;		//前回のステート



	int m_maxHpQuotient = 0;			//最大HPを割った商


};

