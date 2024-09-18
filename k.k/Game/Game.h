#pragma once
//#include "Level3DRender.h"
//#include "Level2DRender.h"

class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;
class EntryBoss;
class BattleStart;
class Pause;
class Brave;
class WaveManager;
class Summoner;

class GameFinishCamera;


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

	


	///////////////////////////////////////////////////////////////////
	//その他の関数
	///////////////////////////////////////////////////////////////////

	
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

	SkyCube* GetSkyCube()
	{
		return m_skyCube;
	}

	void DeleteGameCamera();

	void DeleteThis();

	/// <summary>
	/// プレイヤーとカメラの生成
	/// </summary>
	void CreatePlayerAndCamera();

	/// <summary>
	/// ボスの生成
	/// </summary>
	void CreateBoss();

	void UpdateGameSystem();

	/// <summary>
	/// バトルフェーズクラス生成
	/// </summary>
	void CreateBattleWave();


	void GameOutComeProcess();

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
	/// スカイキューブの初期化
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// BGMを少しずつ小さくする
	/// </summary>
	void CalcMuteBGM();

	//void Render(RenderContext& rc);
	

	EnGameStep					m_enGameStep = enGameStep_FadeOut;

	EnGameEndCameraState		m_clearCameraState = enClearCameraState_None;

	Pause*						m_pause = nullptr;
	Fade*						m_fade = nullptr;

	BossStage1*					m_bossStage1 = nullptr;
	GameCamera*					m_gameCamera = nullptr;
	
	SkyCube*					m_skyCube = nullptr;
	const Vector3				m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI*						m_gameUI = nullptr;
	EntryBoss*					m_entryBoss = nullptr;
	BattleStart*				m_battleStart = nullptr;
	Brave* m_brave = nullptr;
	WaveManager* m_waveManager = nullptr;
	Summoner* m_summoner = nullptr;

	bool						m_ChaseBossFlag = false;						//ボスを見るかのフラグ

	bool						m_playerAnnihilationFlag = false;

	bool						m_bossMovieEndFlag = false;						//ボスの登場ムービーが終わったら

	bool						m_displayResultFlag = false;

	bool						m_muteBGMFlag = false;							//BGMをミュートするかのフラグ
	float						m_bgmVolume = 0.0f;								//BGMの音量

	bool						m_battleBGMFlag = false;

};

