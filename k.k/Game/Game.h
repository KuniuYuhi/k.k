#pragma once
//#include "Level3DRender.h"
//#include "Level2DRender.h"

class Player;
class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;
class EntryBoss;
class BattleStart;
class Pause;
class Boss;
class GameFinishCamera;
class BattlePhase;

class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	
	///////////////////////////////////////////////////////
	//構造体、列挙型の宣言
	///////////////////////////////////////////////////////

	enum EnGameStep
	{
		enGameStep_FadeOut,
		enGameStep_FadeNone,
		enGameStep_Battle
	};
	
	/// <summary>
	/// ゲームが終わった時のカメラを向ける相手
	/// </summary>
	enum EnGameEndCameraState
	{
		enClearCameraState_None,
		enClearCameraState_Lich,	//リッチ
		enClearCameraState_Player	//勇者
	};

	///////////////////////////////////////////////////////
	//進行
    //////////////////////////////////////////////////////
	/// <summary>
	/// ステートの管理
	/// </summary>
	void ManageState();
	//ゲームの流れ
	
	/// <summary>
	/// ゲームスタート
	/// </summary>
	void OnProcessGameStartTransition();
	/// <summary>
	/// ボス登場ムービー
	/// </summary>
	void OnProcessAppearanceBossTransition();
	/// <summary>
	/// インゲーム
	/// </summary>
	void OnProcessGameTransition();
	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void OnProcessGameOverTransition();
	/// <summary>
	/// ゲームクリア
	/// </summary>
	void OnProcessGameClearTransition();
	/// <summary>
	/// ポーズ画面
	/// </summary>
	void OnProcessPauseTransition();
	/// <summary>
	/// ゲームエンド
	/// </summary>
	void OnProcessGameEndTransition();


	void OnProcessGame_FadeOutTransition();
	void OnProcessGame_FadeNoneTransition();
	void OnProcessGame_BattleTransition();

	///////////////////////////////////////////////////////////////////
	//その他の関数
	///////////////////////////////////////////////////////////////////

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
	/// ゲームUI生成
	/// </summary>
	void CreateGameUI();

	/// <summary>
	/// プレイヤーが全滅したかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerAnnihilationFlag(bool flag)
	{
		m_playerAnnihilationFlag = flag;
	}

	/// <summary>
	/// リザルト画面遷移処理
	/// </summary>
	void GoResult();

	/// <summary>
	/// 勝敗が決まったか
	/// </summary>
	/// <returns></returns>
	bool IsWinnerDecision();

	/// <summary>
	/// ボスの登場ムービーが終わったかのフラグ
	/// </summary>
	/// <param name="flag"></param>
	void SetBossMovieFlag(bool flag)
	{
		m_bossMovieEndFlag = flag;
	}
	
	
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

private:
	/// <summary>
	/// ゲームオブジェクトの初期化
	/// </summary>
	void InitGameObject();
	/// <summary>
	/// ライトの初期化
	/// </summary>
	void InitLighting();

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
	/// バトルフェーズクラス生成
	/// </summary>
	void CreateBattlePhase();
	/// <summary>
	/// スカイキューブの初期化
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// BGMを少しずつ小さくする
	/// </summary>
	void CalcMuteBGM();

	

	EnGameStep					m_enGameStep = enGameStep_FadeOut;

	EnGameEndCameraState		m_clearCameraState = enClearCameraState_None;

	BattlePhase*				m_battlePhase = nullptr;
	Pause*						m_pause = nullptr;
	Fade*						m_fade = nullptr;
	Player*						m_player = nullptr;
	Boss*						m_boss = nullptr;

	GameFinishCamera* m_gameFinishCamera = nullptr;

	BossStage1*					m_bossStage1 = nullptr;
	GameCamera*					m_gameCamera = nullptr;
	ResultSeen*					m_result = nullptr;
	SkyCube*					m_skyCube = nullptr;
	const Vector3				m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI*						m_gameUI = nullptr;
	EntryBoss*					m_entryBoss = nullptr;
	BattleStart*				m_battleStart = nullptr;

	ModelRender					model;

	bool						m_ChaseBossFlag = false;						//ボスを見るかのフラグ

	bool						m_playerAnnihilationFlag = false;

	bool						m_bossMovieEndFlag = false;						//ボスの登場ムービーが終わったら

	bool						m_displayResultFlag = false;

	bool						m_muteBGMFlag = false;							//BGMをミュートするかのフラグ
	float						m_bgmVolume = 0.0f;								//BGMの音量

	bool						m_battleBGMFlag = false;

};

