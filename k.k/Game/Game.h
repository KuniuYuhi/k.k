#pragma once
#include "Level3DRender.h"
//#include "Level2DRender.h"

class Player;
class Lich;
class BossStage1;
class GameCamera;
class ResultSeen;


class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();

	void InitSkyCube();

	void GoResult();
	
	void SpriteTransform();

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

	void Render(RenderContext& rc);

private:

	Player* m_player = nullptr;
	Lich* m_lich = nullptr;
	BossStage1* m_bossStage1 = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ResultSeen* m_result = nullptr;
	SkyCube* m_skyCube = nullptr;
	const Vector3 m_skyPos = { 0.0f,-700.0f,0.0f };

	FontRender fontTest;
	float m_fontScale = 1.0f;

	//Level2DRender level2DSp;
	SpriteRender spriteTest;
	float wipSize = 5.0f;

	//spotLight
	Vector3 spPosition = Vector3::Zero;
	Vector3 spDirection = Vector3::Zero;

	Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_DeathBossFlag = false;
	bool m_createResultFlag = false;

	bool m_playerAnnihilationFlag = false;
};

