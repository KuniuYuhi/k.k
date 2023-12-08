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
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �����蔻��̐ݒ�
	/// </summary>
	void SettingCollision();

	/// <summary>
	/// ��������
	/// </summary>
	void Explosion();

	/// <summary>
	/// �U���͂�ݒ�
	/// </summary>
	/// <param name="attack"></param>
	void SetAtk(const int attack)
	{
		m_attack = attack;
	}

	/// <summary>
	/// �U���͂�Ԃ�
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

	const int				m_moveLimitTimer = 5;		//�ړ��̐�������

	int				m_attack = 0;



};

