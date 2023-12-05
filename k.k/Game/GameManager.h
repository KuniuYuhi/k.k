#pragma once

/*
*@brief 進行状況の管理
*/



class GameManager :public Noncopyable
{
public:
	//////////////////////////////////////////////////////////////
	//構造体、列挙型の宣言
	//////////////////////////////////////////////////////////////
	
	/// <summary>
	/// ゲームのシーン
	/// </summary>
	enum EnGameSeenState
	{
		enGameSeenState_Fade,
		enGameSeenState_GameStart,
		enGameSeenState_AppearanceBoss,
		enGameSeenState_Game,
		enGameSeenState_Pause,
		enGameSeenState_GameOver,
		enGameSeenState_GameClear
	};
	/// <summary>
	/// 勝敗ステート
	/// </summary>
	enum EnOutComeState
	{
		enOutComeState_None,
		enOutComeState_PlayerWin,
		enOutComeState_PlayerLose,
	};
	/// <summary>
	/// フェーズステート。ループする
	/// </summary>
	enum EnPhaseState
	{
		EnPhaseState_Phase1,		//フェーズ1
		EnPhaseState_Phase2,		//フェーズ2
		EnPhaseState_Phase3,		//フェーズ3
		EnPhaseState_BreakTime,		//休憩
	};

private:
	GameManager(EnGameSeenState startGameSeenState);
	~GameManager();
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();


	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance(EnGameSeenState startGameSeenState)
	{
		m_instance = new GameManager(startGameSeenState);
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// ゲームシーンステートを設定
	/// </summary>
	/// <param name="setGameSeenState"></param>
	void SetGameSeenState(EnGameSeenState setGameSeenState)
	{
		m_enGameSeenState = setGameSeenState;
	}
	/// <summary>
	/// ゲームシーンステートを設定
	/// </summary>
	/// <returns></returns>
	const EnGameSeenState& GetGameSeenState() const
	{
		return m_enGameSeenState;
	}

	/// <summary>
	/// 勝敗結果ステートを設定
	/// </summary>
	/// <param name="outComeState"></param>
	void SetOutComeState(EnOutComeState outComeState)
	{
		m_enOutComeState = outComeState;
	}
	/// <summary>
	/// 勝敗結果ステートを取得
	/// </summary>
	/// <returns></returns>
	const EnOutComeState& GetOutComeState() const
	{
		return m_enOutComeState;
	}

	/// <summary>
	/// 現在のフェーズを取得
	/// </summary>
	/// <returns></returns>
	const EnPhaseState& GetNowPhaseState() const
	{
		return m_enPhaseState;
	}



	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	void Execute();

	/// <summary>
	/// バトルスタートクラスの作成
	/// </summary>
	void CreateBattleStartClass();

	/// <summary>
	/// プレイヤークラス生成
	/// </summary>
	void CreatePlayerClass();
	/// <summary>
	/// プレイヤークラス削除
	/// </summary>
	void DeletePlayerClass();

	/// <summary>
	/// ボスクラス生成
	/// </summary>
	void CreateBoss();
	/// <summary>
	/// ボスクラス削除
	/// </summary>
	void DeleteBossClass();

	/// <summary>
	/// ゲームの現在のステートと検索したいステートが同じか判定
	/// </summary>
	/// <param name="searchGameState">検索したいゲームステート</param>
	/// <returns>trueで同じ、falseで違う</returns>
	/*bool IsMatchGameState(EnGameState searchGameState)
	{
		if (GetNowGameState() == searchGameState)
		{
			return true;
		}
		return false;
	}*/

	

	/// <summary>
	/// 秒の取得
	/// </summary>
	/// <returns></returns>
	const int GetMinute() const
	{
		return m_minute;
	}
	/// <summary>
	/// 秒の取得
	/// </summary>
	/// <returns></returns>
	const int GetSecond() const
	{
		return m_second;
	}

	/// <summary>
	/// プレイヤーの勝利フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerWinFlag(bool flag)
	{
		m_playerWinFlag = flag;
	}
	const bool& GetPlayerWinFlag() const
	{
		return m_playerWinFlag;
	}
	/// <summary>
	/// プレイヤーの敗北フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerLoseFlag(bool flag)
	{
		m_playerLoseFlag = flag;
	}
	const bool& GetPlayerLoseFlag() const
	{
		return m_playerLoseFlag;
	}



private:

	/// <summary>
	/// フェーズの処理
	/// </summary>
	void ProcessPhase();

	/// <summary>
	/// フェーズ1,2,3の処理
	/// </summary>
	void OnProcessPhaseTransition();
	/// <summary>
	/// 休憩時間の処理
	/// </summary>
	void OnProcessBreakTimeTransition();

	/// <summary>
	/// 制限時間の計算
	/// </summary>
	void CalcTimeLimmit();
	/// <summary>
	/// 勝敗が着いたか
	/// </summary>
	void IsOutComeDecided();


public:

	static GameManager* m_instance;		//唯一のインスタンスのアドレスを記録する変数。

private:


	EnGameSeenState m_enGameSeenState = enGameSeenState_GameStart;
	EnOutComeState m_enOutComeState = enOutComeState_None;
	EnPhaseState m_enPhaseState = EnPhaseState_Phase1;

	float m_minute = 3.0f;			//制限時間
	float m_second = 0.0f;

	bool m_playerWinFlag = false;
	bool m_playerLoseFlag = false;

	float m_breakTimeTimer = 0.0f;

	float m_phaseTimer = 0.0f;

};



