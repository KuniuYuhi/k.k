#pragma once
#include "MagicBall.h"

class Lich;

class DarkBall:public MagicBall
{
public:
	DarkBall();
	~DarkBall();

	bool Start();
	void Update();

	void Move();

	void SettingCollision();

	void Explosion();

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

	/// <summary>
	/// ���b�`�̃I�u�W�F�N�g�̐ݒ�
	/// </summary>
	/// <param name="lich"></param>
	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}


private:
	Lich*					m_lich = nullptr;
	CollisionObject*		m_BallCollision;

	EffectEmitter*			m_darkBallEffect;

	Vector3					m_collisionPosition = g_vec3Zero;
	Vector3					m_forLichBallScale = { 2.3f,2.3f,2.3f };

	const int				m_moveLimitTimer = 5;		//�ړ��̐�������

	int				m_attack = 0;



};

