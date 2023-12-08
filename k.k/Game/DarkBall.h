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
	/// ˆÚ“®ˆ—
	/// </summary>
	void Move();

	/// <summary>
	/// “–‚½‚è”»’è‚Ìİ’è
	/// </summary>
	void SettingCollision();

	/// <summary>
	/// ”š”­ˆ—
	/// </summary>
	void Explosion();

	/// <summary>
	/// UŒ‚—Í‚ğİ’è
	/// </summary>
	/// <param name="attack"></param>
	void SetAtk(const int attack)
	{
		m_attack = attack;
	}

	/// <summary>
	/// UŒ‚—Í‚ğ•Ô‚·
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

	const int				m_moveLimitTimer = 5;		//ˆÚ“®‚Ì§ŒÀŠÔ

	int				m_attack = 0;



};

