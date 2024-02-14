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
	/// ��Ԃ��Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void OnProcessMoveTransition();
	/// <summary>
	/// ��������
	/// </summary>
	void OnProcessExplosionTransition();

	/// <summary>
	/// ��������܂ł̎��Ԃ��v�Z
	/// </summary>
	void CalcDeleteTime();

	/// <summary>
	/// �����̍��W�Ƃ̋����ɂ���ă_���[�W��ς���
	/// </summary>
	int CalcDamageToDistance(const Vector3 targetPosition);

	/// <summary>
	/// ���e�I�̈ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// ���������蔻��̐���
	/// </summary>
	void CreateExplosionCollision();

	/// <summary>
	/// �n�ʂɐG�ꂽ���̔���
	/// </summary>
	/// <param name="downValue">���Ƀ��C���΂��̂ŁA�ǂ̂��炢�����邩</param>
	/// <returns>�G�ꂽ��true</returns>
	bool IsGroundCheck(const float downValue);

	/// <summary>
	/// �v���C���[�����e�I�ɐڐG�����狭���I�ɔ���������
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
	/// �_���[�W��^����U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int GetAttack() override;

private:

	/// <summary>
	/// ���e�I�̃G�t�F�N�g������
	/// </summary>
	void PlayMeteoEffect();

	/// <summary>
	/// ���e�I�̃��[�g(��������`������)�̐ݒ�
	/// </summary>
	void SettingMeteoRoute();

	/// <summary>
	/// �U���͈͂̃G�t�F�N�g���v���C
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
	Vector3						m_startPosition = g_vec3Zero;		//�n�_
	Vector3						m_centerPosition = g_vec3Zero;	//���ԓ_
	Vector3						m_targetPosition = g_vec3Zero;		//�I�_

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

