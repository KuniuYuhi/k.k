#pragma once
#include "IAttackObject.h"

class Meteo:public IAttackObject
{
public:
	Meteo();
	~Meteo();

	enum EnExplosionState
	{
		enMoveState,
		enExplosionState
	};

	bool Start() override;
	void Update() override;
	
	/// <summary>
	/// 状態を管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// 移動処理
	/// </summary>
	void OnProcessMoveTransition();
	/// <summary>
	/// 爆発処理
	/// </summary>
	void OnProcessExplosionTransition();

	/// <summary>
	/// 消去するまでの時間を計算
	/// </summary>
	void CalcDeleteTime();

	/// <summary>
	/// 引数の座標との距離によってダメージを変える
	/// </summary>
	int CalcDamageToDistance(const Vector3 targetPosition);

	/// <summary>
	/// メテオの移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 爆発当たり判定の生成
	/// </summary>
	void CreateExplosionCollision();

	/// <summary>
	/// 地面に触れたかの判定
	/// </summary>
	/// <param name="downValue">下にレイを飛ばすので、どのくらい下げるか</param>
	/// <returns>触れたらtrue</returns>
	bool IsGroundCheck(const float downValue);

	/// <summary>
	/// プレイヤーがメテオに接触したら強制的に爆発させる
	/// </summary>
	void Explosion();

	void SetPosition(Vector3 position)
	{
		m_position = position;
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

	/// <summary>
	/// ダメージを与える攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int GetAttack() override;

private:

	/// <summary>
	/// メテオのエフェクト初期化
	/// </summary>
	void PlayMeteoEffect();

	/// <summary>
	/// メテオのルート(放物線を描くため)の設定
	/// </summary>
	void SettingMeteoRoute();

	/// <summary>
	/// 攻撃範囲のエフェクトをプレイ
	/// </summary>
	void PlayRangeEffect();

private:

	EffectEmitter*				m_meteoEffect = nullptr;
	EffectEmitter*				m_ExplosionEffect = nullptr;
	EffectEmitter*				m_rangeEffect = nullptr;

	CollisionObject*			m_collision = nullptr;
	CollisionObject*			m_explosionCollision = nullptr;


	Vector3						m_meteoVerocity = g_vec3Zero;
	Vector3						m_forward = g_vec3Zero;

	Vector3						m_scale = g_vec3One;
	Vector3						m_moveSpeed = g_vec3Zero;
	Vector3						m_position = g_vec3Zero;
	Vector3						m_movePos = g_vec3Zero;
	Vector3						m_startPosition = g_vec3Zero;		//始点
	Vector3						m_centerPosition = g_vec3Zero;	//中間点
	Vector3						m_targetPosition = g_vec3Zero;		//終点

	Vector3						StartToCenter = g_vec3Zero;
	Vector3						CenterToEnd = g_vec3Zero;

	EnExplosionState			m_state = enMoveState;

	float						m_flightDuration = 0.0f;

	bool						m_explosionFlag = false;

	const float					m_speed = 200.0f;

	float						m_timer = 0.0f;

	const float					m_explosionEndTime = 0.5f;
	float						m_explosionEndTimer = 0.0f;

	bool						m_explosionEffectFlag = false;

	float						m_angle = 0.0f;
	float						m_deleteTimer = 0.0f;
};

