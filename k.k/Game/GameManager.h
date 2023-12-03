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
	//ゲームのシーン
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

	enum EnOutComeState
	{
		enOutComeState_None,
		enOutComeState_PlayerWin,
		enOutComeState_PlayerLose,
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
	/// ボスクラス生成
	/// </summary>
	void CreateBoss();




	/// <summary>
	/// 制限時間の計算
	/// </summary>
	void CalcTimeLimmit();

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


public:
	
	EnGameSeenState m_enGameSeenState = enGameSeenState_GameStart;
	EnOutComeState m_enOutComeState = enOutComeState_None;


	static GameManager* m_instance;		//唯一のインスタンスのアドレスを記録する変数。

private:
	float m_minute = 3.0f;			//制限時間
	float m_second = 0.0f;

	bool m_playerWinFlag = false;
	bool m_playerLoseFlag = false;

};



