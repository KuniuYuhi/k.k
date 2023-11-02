#pragma once


class Meteo:public IGameObject
{
public:
	Meteo();
	~Meteo();

	enum EnExplosionState
	{
		enMoveState,
		enExplosionState
	};

	bool Start();
	void Update();
	
	void ManageState();

	void OnProcessMoveTransition();
	void OnProcessExplosionTransition();


	void CalcDeleteTime();

	/// <summary>
	/// 引数の座標との距離によってダメージを変える
	/// </summary>
	int CalcDamageToDistance(const Vector3 targetPosition);

	void Move();

	void CreateExplosionCollision();


	bool IsGroundCheck(const float downValue);

	/// <summary>
	/// プレイヤーがメテオに接触したら強制的に爆発させる
	/// </summary>
	void Explosion();

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

	ModelRender					m_model;
	EffectEmitter*				m_meteoEffect;
	EffectEmitter*				m_ExplosionEffect;

	CollisionObject*			m_collision;
	CollisionObject*			m_explosionCollision;

	Vector3						m_scale = g_vec3One;
	Vector3						m_moveSpeed = g_vec3Zero;
	Vector3						m_position = g_vec3Zero;
	Vector3						m_movePos = g_vec3Zero;
	Vector3						m_startPosition = g_vec3Zero;		//始点
	Vector3						m_centerPosition = g_vec3Zero;	//中間点
	Vector3						m_targetPosition = g_vec3Zero;		//終点
	Quaternion					m_rotation = Quaternion::Identity;
	Quaternion					m_addRotation = Quaternion::Identity;

	Vector3						StartToCenter = g_vec3Zero;
	Vector3						CenterToEnd = g_vec3Zero;

	EnExplosionState			m_state;

	bool						m_explosionFlag = false;

	const float					m_speed = 200.0f;

	float						m_timer = 0.0f;

	const float					m_explosionEndTime = 0.5f;
	float						m_explosionEndTimer = 0.0f;
	
	const int					m_attack = 60;
	const int					m_explosionAttack = 40;

	bool						m_explosionEffectFlag = false;
};

