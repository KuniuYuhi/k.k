#pragma once
#include "MagicBall.h"

class Player;

class DarkBall:public MagicBall
{
public:
	DarkBall();
	~DarkBall();

	bool Start() override;
	void Update() override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 当たり判定の設定
	/// </summary>
	void SettingCollision();

	/// <summary>
	/// 爆発処理
	/// </summary>
	void Explosion();

	/// <summary>
	/// ダークボールのエフェクト再生
	/// </summary>
	void PlayDarkBallEffect();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsHit();

private:

	CollisionObject* m_BallCollision = nullptr;

	EffectEmitter* m_darkBallEffect = nullptr;

	Player* m_player = nullptr;

	Vector3					m_collisionPosition = g_vec3Zero;
	
	

	const int				m_moveLimitTimer = 5;		//移動の制限時間

};

