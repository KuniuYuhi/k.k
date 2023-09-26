#pragma once

class Lich;

class DarkWall:public IGameObject
{
public:
	DarkWall();
	~DarkWall();

	bool Start();
	void Update();
	
	/// <summary>
	/// çsóÒÇç¿ïWÇ…ìKâûÇ∑ÇÈ
	/// </summary>
	Vector3 AppltMatrixToPosition();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	void SetAtk(int attack)
	{
		m_attak = attack;
	}

	const int GetAtk() const
	{
		return m_attak;
	}

private:
	Lich*							m_lich = nullptr;
	EffectEmitter*					m_darkWallEffect = nullptr;

	Vector3							m_position = Vector3::Zero;
	Vector3							m_collisionPosition = Vector3::Zero;
	Quaternion						m_rotation = Quaternion::Identity;

	CollisionObject*				m_collision;

	int								m_attak = 20;

	const float						m_speed = 50.0f;

	const float						m_deleteTime = 2.0f;
	float							m_deleteTimer = 0.0f;

};

