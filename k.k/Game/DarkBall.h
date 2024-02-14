#pragma once
#include "MagicBall.h"

class Player;

class DarkBall:public MagicBall
{
public:
	DarkBall();
	~DarkBall();

	bool Start() override;
	void Update() override;

private:
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
	/// �_�[�N�{�[���̃G�t�F�N�g�Đ�
	/// </summary>
	void PlayDarkBallEffect();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsHit();

private:

	CollisionObject* m_BallCollision = nullptr;

	EffectEmitter* m_darkBallEffect = nullptr;

	Player* m_player = nullptr;

	Vector3					m_collisionPosition = g_vec3Zero;
	
	

	const int				m_moveLimitTimer = 5;		//�ړ��̐�������

};

