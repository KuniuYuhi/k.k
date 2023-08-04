#pragma once


class Meteo:public IGameObject
{
public:
	Meteo();
	~Meteo();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Explosion();

	void Move();

	void Rotation();

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	void SetTargetPosition(Vector3 targetpos)
	{
		m_targetPosition = targetpos;
	}

	void SetExplosionFlag(bool flag)
	{
		m_explosionFlag = flag;
	}

	const bool GetExplosionFlag() const
	{
		return m_explosionFlag;
	}

	const int& GetAtk() const
	{
		return m_attack;
	}

	const int& GetExplosionAttack() const
	{
		return m_explosionAttack;
	}

private:

	ModelRender m_model;

	CollisionObject* m_collision;
	CollisionObject* m_explosionCollision;

	Vector3 m_scale = g_vec3One;
	Vector3 m_moveSpeed = g_vec3Zero;
	Vector3 m_position = g_vec3Zero;
	Vector3 m_movePos = g_vec3Zero;
	Vector3 m_startPosition = g_vec3Zero;		//始点
	Vector3 m_centerPosition = g_vec3Zero;	//中間点
	Vector3 m_targetPosition = g_vec3Zero;		//終点
	Quaternion m_rotation = Quaternion::Identity;
	Quaternion m_addRotation = Quaternion::Identity;

	Vector3 StartToCenter = g_vec3Zero;
	Vector3 CenterToEnd = g_vec3Zero;

	bool m_explosionFlag = false;

	const float m_speed = 200.0f;

	const float m_yUp = 800.0f;


	float m_timer = 0.0f;

	const float m_explosionEndTime = 1.0f;
	float m_explosionEndTimer = 0.0f;

	const int m_attack = 50;
	const int m_explosionAttack = 40;


};

