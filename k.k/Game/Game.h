#pragma once
#include "Level3DRender.h"
//#include "Level2DRender.h"

class Player;
class Lich;
class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;

class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// フェード
	/// </summary>
	/// <returns></returns>
	bool Fadecomplete();

	void CreateBoss();

	void InitSkyCube();

	void GoResult();

	void Spotmove();

	const Vector3 GetPosition() const
	{
		return m_position;
	}

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

	enum EnGameState
	{
		enGameState_Fade,
		enGameState_GameStart,
		enGameState_AppearanceBoss,
		enGameState_Game,
		enGameState_Pause,
		enGameState_GameOver,
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

private:
	Fade* m_fade = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;
	BossStage1* m_bossStage1 = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ResultSeen* m_result = nullptr;
	SkyCube* m_skyCube = nullptr;
	const Vector3 m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI* m_gameUI = nullptr;


	ModelRender model;


	//Level2DRender level2DSp;

	//spotLight
	Vector3 spPosition = Vector3::Zero;
	Vector3 spDirection = Vector3::Zero;

	Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	EnGameState m_enGameState = enGameState_GameStart;

	bool m_DeathBossFlag = false;
	bool m_createResultFlag = false;

	bool m_playerAnnihilationFlag = false;
};

