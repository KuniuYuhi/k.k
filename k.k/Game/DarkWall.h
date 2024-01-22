#pragma once
#include "IAttackObject.h"

class Summoner;

class DarkWall:public IAttackObject
{
public:
	DarkWall();
	~DarkWall();

	bool Start();
	void Update();
	
	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	void PlayDarkWallEffect();
	/// <summary>
	/// �����蔻�萶��
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// �s������W�ɓK������
	/// </summary>
	Vector3 AppltMatrixToPosition();

private:
	Summoner*							m_summoner = nullptr;
	EffectEmitter*					m_darkWallEffect = nullptr;

	Vector3							m_position = Vector3::Zero;
	Vector3							m_collisionPosition = Vector3::Zero;
	Quaternion						m_rotation = Quaternion::Identity;

	CollisionObject*				m_collision;

	const float						m_speed = 50.0f;

	const float						m_deleteTime = 2.0f;
	float							m_deleteTimer = 0.0f;

	int m_darkWallBoonId = -1;

};

