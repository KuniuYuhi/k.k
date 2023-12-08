#pragma once
#include "MagicBall.h"

class DarkBall:public MagicBall
{
public:
	DarkBall();
	~DarkBall();

	bool Start();
	void Update();

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
	/// 攻撃力を設定
	/// </summary>
	/// <param name="attack"></param>
	void SetAtk(const int attack)
	{
		m_attack = attack;
	}

	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	/// <returns></returns>
	const int& GetBallAtk() const
	{
		return m_attack;
	}


private:

	CollisionObject*		m_BallCollision;

	EffectEmitter*			m_darkBallEffect;

	Vector3					m_collisionPosition = g_vec3Zero;
	Vector3					m_forLichBallScale = { 2.3f,2.3f,2.3f };

	const int				m_moveLimitTimer = 5;		//移動の制限時間

	int				m_attack = 0;



};

