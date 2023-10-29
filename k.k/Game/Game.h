#pragma once
//#include "Level3DRender.h"
//#include "Level2DRender.h"

class Player;
class Lich;
class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;
class EntryBoss;
class BattleStart;

class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	
	/// <summary>
	/// ゲームの勝敗が決まったら
	/// </summary>
	/// <returns></returns>
	bool IsOutcome();
	/// <summary>
	/// 勝利時の処理
	/// </summary>
	void ProcessWin();
	/// <summary>
	/// 負けの処理
	/// </summary>
	void ProcessLose();
	/// <summary>
	/// タイムアップで負けた時の処理
	/// </summary>
	void ProcessLoseTimeUp();

	/// <summary>
	/// ボスがやられたかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDeathBossFlag(bool flag)
	{
		m_DeathBossFlag = flag;
	}

	/// <summary>
	/// プレイヤーが全滅したかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerAnnihilationFlag(bool flag)
	{
		m_playerAnnihilationFlag = flag;
	}

	/// <summary>
	/// ボス登場ムービーのスキップ処理
	/// </summary>
	/// <returns></returns>
	bool IsBossMovieSkipTime();

	/// <summary>
	/// 勝敗ステート
	/// </summary>
	enum EnOutCome
	{
		enOutCome_Player_Win,
		enOutCome_Player_Lose,
		enOutCome_None
	};
	/// <summary>
	/// リザルト画面遷移処理
	/// </summary>
	/// <param name="outcome">勝敗ステート</param>
	void GoResult(EnOutCome outcome);
	/// <summary>
	/// 勝敗ステートの設定
	/// </summary>
	/// <param name="outCome"></param>
	void SetEnOutCome(EnOutCome outCome)
	{
		m_enOutCome = outCome;
	}
	/// <summary>
	/// 勝敗ステートの取得
	/// </summary>
	/// <returns></returns>
	const EnOutCome& GetEnOutCome() const
	{
		return m_enOutCome;
	}

	enum EnGameState
	{
		enGameState_Fade,
		enGameState_GameStart,
		enGameState_AppearanceBoss,
		enGameState_Game,
		enGameState_Pause,
		enGameState_GameOver,
		enGameState_GameOver_TimeUp,
		enGameState_GameClear
	};

	/// <summary>
	/// 次のゲームステートを設定
	/// </summary>
	/// <param name="nextgamestate"></param>
	void SetNextGameState(EnGameState nextgamestate)
	{
		m_enGameState = nextgamestate;
	}
	/// <summary>
	/// ゲームステートを取得
	/// </summary>
	/// <returns></returns>
	EnGameState GetNowGameState()
	{
		return m_enGameState;
	}

	/// <summary>
	/// タイムアップか
	/// </summary>
	/// <returns>タイムアップならtrueを返す</returns>
	bool IsTimeUp()
	{
		return m_enGameState == enGameState_GameOver_TimeUp;
	}
	/// <summary>
	/// 勝敗が決まったか
	/// </summary>
	/// <returns></returns>
	bool IsWinnerDecision()
	{
		return m_enGameState == enGameState_GameOver_TimeUp ||
			m_enGameState == enGameState_GameOver ||
			m_enGameState == enGameState_GameClear;
	}

	/// <summary>
	/// ボスの登場ムービーが終わったかのフラグ
	/// </summary>
	/// <param name="flag"></param>
	void SetBossMovieFlag(bool flag)
	{
		m_bossMovieEndFlag = flag;
	}
	/// <summary>
	/// ボスが死んだかのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetDeathBossFlag()
	{
		return m_DeathBossFlag;
	}

	/// <summary>
	/// ゲームが終わった時のカメラを向ける相手
	/// </summary>
	enum EnGameEndCameraState
	{
		enClearCameraState_None,
		enClearCameraState_Lich,	//リッチ
		enClearCameraState_Player	//勇者
	};
	/// <summary>
	/// ゲームエンドカメラを設定
	/// </summary>
	/// <param name="nextClearCameraState"></param>
	void SetClearCameraState(EnGameEndCameraState nextClearCameraState)
	{
		m_clearCameraState = nextClearCameraState;
	}
	/// <summary>
	/// ゲームエンドカメラを取得
	/// </summary>
	/// <returns></returns>
	EnGameEndCameraState GetClearCameraState()
	{
		return m_clearCameraState;
	}

	/// <summary>
	/// 制限時間の分の取得
	/// </summary>
	/// <returns></returns>
	const float& GetMinute() const
	{
		return m_minute;
	}
	/// <summary>
	/// 制限時間の秒の取得
	/// </summary>
	/// <returns></returns>
	const float& GetSecond() const
	{
		return m_second;
	}

	/// <summary>
	/// ゲームの現在のステートと検索したいステートが同じか判定
	/// </summary>
	/// <param name="searchGameState">検索したいゲームステート</param>
	/// <returns>trueで同じ、falseで違う</returns>
	bool IsMatchGameState(EnGameState searchGameState)
	{
		if (GetNowGameState() == searchGameState)
		{
			return true;
		}
		return false;
	}


private:

	/// <summary>
	/// フェード
	/// </summary>
	/// <returns></returns>
	bool Fadecomplete();
	/// <summary>
	/// ボスの生成
	/// </summary>
	void CreateBoss();
	/// <summary>
	/// スカイキューブの初期化
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// 制限時間の計算
	/// </summary>
	/// <returns></returns>
	bool CalcTimeLimit();
	/// <summary>
	/// BGMを少しずつ小さくする
	/// </summary>
	void CalcMuteBGM();

	/// <summary>
	/// ステートの管理
	/// </summary>
	void ManageState();
	//ゲームの流れ
	void OnProcessGameStartTransition();
	void OnProcessAppearanceBossTransition();
	void OnProcessGameTransition();
	void OnProcessGameOverTransition();
	void OnProcessGameClearTransition();

	enum EnGameStep
	{
		enGameStep_FadeOut,
		enGameStep_FadeNone,
		enGameStep_Battle
	};
	

	void OnProcessGame_FadeOutTransition();
	void OnProcessGame_FadeNoneTransition();
	void OnProcessGame_BattleTransition();

	enum EnFadeState
	{
		enFadeState_None,
		enFadeState_StartToBoss,
		enFadeState_BossToPlayer,

	};


	EnFadeState					m_enFadeState = enFadeState_None;

	EnGameStep					m_enGameStep = enGameStep_FadeOut;

	EnGameEndCameraState		m_clearCameraState = enClearCameraState_None;

	Fade*						m_fade = nullptr;
	Player*						m_player = nullptr;
	Lich*						m_lich = nullptr;
	BossStage1*					m_bossStage1 = nullptr;
	GameCamera*					m_gameCamera = nullptr;
	ResultSeen*					m_result = nullptr;
	SkyCube*					m_skyCube = nullptr;
	const Vector3				m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI*						m_gameUI = nullptr;
	EntryBoss*					m_entryBoss = nullptr;
	BattleStart*				m_battleStart = nullptr;

	ModelRender					model;


	//Level2DRender level2DSp;

	Vector3						m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion					m_rotation = Quaternion::Identity;
	Vector3						m_scale = Vector3::One;

	EnGameState					m_enGameState = enGameState_GameStart;

	EnOutCome					m_enOutCome = enOutCome_None;

	float						m_TimeLimit = 300.0f;							//制限時間三分
	float						m_minute = 3.0f;								//分
	float						m_second = 0.0f;								//秒


	bool						m_DeathBossFlag = false;						//ボスがやられたのフラグ
	bool						m_ChaseBossFlag = false;						//ボスを見るかのフラグ

	bool						m_playerAnnihilationFlag = false;

	bool						m_bossMovieEndFlag = false;						//ボスの登場ムービーが終わったら

	bool						m_displayResultFlag = false;
	
	const float					m_fadeTime = 3.0f;
	const float					m_cameraZoomOutTime = 2.0f;
	float						m_cameraZoomOutTimer = 0.0f;

	const float					m_bossMovieSkipTime = 1.0f;
	float						m_bossMovieSkipTimer = 0.0f;

	bool						m_muteBGMFlag = false;							//BGMをミュートするかのフラグ
	float						m_bgmVolume = 0.0f;								//BGMの音量

	bool						m_battleBGMFlag = false;

};

