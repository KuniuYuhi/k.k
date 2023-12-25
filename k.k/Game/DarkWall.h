#pragma once

class Summoner;

class DarkWall:public IGameObject
{
public:
	DarkWall();
	~DarkWall();

	bool Start();
	void Update();
	
	/// <summary>
	/// エフェクトの再生
	/// </summary>
	void PlayDarkWallEffect();
	/// <summary>
	/// 当たり判定生成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 行列を座標に適応する
	/// </summary>
	Vector3 AppltMatrixToPosition();

	void SetAtk(int attack)
	{
		m_attak = attack;
	}

	const int GetAtk() const
	{
		return m_attak;
	}

private:
	Summoner*							m_summoner = nullptr;
	EffectEmitter*					m_darkWallEffect = nullptr;

	Vector3							m_position = Vector3::Zero;
	Vector3							m_collisionPosition = Vector3::Zero;
	Quaternion						m_rotation = Quaternion::Identity;

	CollisionObject*				m_collision;

	int								m_attak = 20;

	const float						m_speed = 50.0f;

	const float						m_deleteTime = 2.0f;
	float							m_deleteTimer = 0.0f;

	int m_darkWallBoonId = -1;

};

