#pragma once

/*
*@brief 進行状況の管理
*/

#include "AllGameSceneState.h"

class GameSceneContext;

/// <summary>
/// シングルトンパターンで作成
/// </summary>
class GameSceneManager:public Noncopyable
{
private:
	GameSceneManager();
	~GameSceneManager();

	/// <summary>
	/// 時間
	/// </summary>
	struct Timer
	{
		void SetMinute(float Minute) { minute = Minute; }

		void SetSecond(float Second) { second = Second; }

		void SubMinute(float value) { minute -= value;  }

		void SubSecond(float value) { second -= value; }

		float GetMinute() { return minute; }
		float GetSecond() {	return second; }

		float minute = 0.0f;			//分
		float second = 0.0f;			//秒
	};


public:

	/// <summary>
	/// 勝敗
	/// </summary>
	enum EnBattleOutCome
	{
		enBattleOutCome_PlayerWin,
		enBattleOutCome_PlayerLose,
		enBattleOutCome_None
	};



	static GameSceneManager* m_instance; //唯一のインスタンスのアドレスを記録する変数。
	
	/// <summary>
	/// インスタンスを生成。ゲームシーンを設定
	/// </summary>
	/// <param name="setGameSceneState"></param>
	static void CreateInstanceAndSetGameSceneState(EnGameSceneState setGameSceneState);

	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static GameSceneManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// ゲームシーンコンテキストを生成
	/// </summary>
	/// <param name="setGameSceneState"></param>
	void CreateGameSceneContext(EnGameSceneState setGameSceneState);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="cangeGameSceneState"></param>
	void ChangeGameSceneState(EnGameSceneState cangeGameSceneState);


	/// <summary>
	/// シーン切り替え可能かフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsSceneChangeableFlag(bool flag)
	{
		m_isSceneChangeable = flag;
	}

	/// <summary>
	/// シーン切り替え可能かフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsSceneChangeableFlag() const
	{
		return m_isSceneChangeable;
	}

	/// <summary>
	/// 現在のゲームシーンステートを取得する
	/// </summary>
	/// <returns></returns>
	const EnGameSceneState& GetCurrentGameSceneState();

	/// <summary>
	/// ゲームタイマーをストップする
	/// </summary>
	void StopGameTimer();


	/// <summary>
	/// バトルの勝敗を設定する
	/// </summary>
	/// <param name="setBattleOutCome"></param>
	void SetBattleOutCome(EnBattleOutCome setBattleOutCome)
	{
		m_battleOutCome = setBattleOutCome;
	}
	/// <summary>
	/// プレイヤーの勝ち
	/// </summary>
	void SetOutComePlayerWin()
	{
		m_battleOutCome = enBattleOutCome_PlayerWin;
	}
	/// <summary>
	/// プレイヤーの負け
	/// </summary>
	void SetOutComPlayerLose()
	{
		m_battleOutCome = enBattleOutCome_PlayerLose;
	}

	/// <summary>
	/// プレイヤーが勝ったか？
	/// </summary>
	/// <returns></returns>
	bool IsPlayerWin()
	{
		return m_battleOutCome == enBattleOutCome_PlayerWin;
	}
	/// <summary>
	/// エネミーたちが勝ったか？
	/// </summary>
	/// <returns></returns>
	bool IsEnemyWin()
	{
		return m_battleOutCome == enBattleOutCome_PlayerLose;
	}

	/// <summary>
	/// 勝敗が着いたか？
	/// </summary>
	/// <returns>着いたならtrue</returns>
	bool IsGameOutcome()
	{
		return m_battleOutCome != enBattleOutCome_None;
	}

	/// <summary>
	/// バトルの勝敗を取得する
	/// </summary>
	/// <returns></returns>
	const EnBattleOutCome& GetBattleOutCome()
	{
		return m_battleOutCome;
	}

	/// <summary>
	/// ボスを消去しました
	/// </summary>
	void BossDelete()
	{
		m_isBossDelete = true;
	}

	void ResetBossDelete()
	{
		m_isBossDelete = false;
	}

	/// <summary>
	/// ボスが消去されたか
	/// </summary>
	/// <returns></returns>
	bool IsBossDelete()
	{
		return m_isBossDelete;
	}

	/// <summary>
	/// タイマーをスタートさせる
	/// </summary>
	void StartGameTimer();


	float GetGameTimerSecond()
	{
		return m_gameTimer.GetSecond();
	}

	float GetGameTimerMinute()
	{
		return m_gameTimer.GetMinute();
	}


private:

	/// <summary>
	/// タイマーを更新
	/// </summary>
	void UpdateGameTimer();

	/// <summary>
	/// ゲームタイマーを計算
	/// </summary>
	void CalcGameTimer();


private:

	std::unique_ptr<GameSceneContext> m_gameSceneContext = nullptr;		//ゲームシーンコンテキスト
																		//をスマートポインタで生成

	bool m_isSceneChangeable = false;				//シーンが切り替え可能か


	bool m_isBossDelete = false;					//ボスは消去されたか？

	EnBattleOutCome m_battleOutCome = enBattleOutCome_None;

	Timer m_gameTimer;

	bool m_isStartGameTimer = false;

	bool m_isTimeOver = false;
	

};

