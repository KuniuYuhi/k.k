#pragma once

#include "Wizard.h"

class FireBall:public IGameObject
{
public:
	FireBall();
	~FireBall();

	bool Start();

	void Update();

	void Timer();

	void Move();

	void Render(RenderContext& rc);



	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

	/// <summary>
	/// �U���͂̎擾
	/// </summary>
	/// <returns></returns>
	int GetAtk() const
	{
		return m_atk;
	}
	
	/// <summary>
	/// �G�ɓ����������̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetHitEnemeyFlag(bool flag)
	{
		m_hitEnemeyFlag = flag;
	}

private:
	Wizard* m_wizard = nullptr;

	ModelRender m_model;
	CollisionObject* m_fireBallCollision;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_collisionPosition= Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	//�U����
	const int m_atk = 30;

	//�t�@�C���[�{�[���̃X�s�[�h
	const float m_speed = 150.0f;
	//�{�[���̐�������
	const int m_limitTimer = 3;
	//�^�C�}�[
	float m_moveTime = 0.0f;
	//�v���C���[����{�[���̋���
	const float m_distance = 60.0f;

	//�G�ɓ����������̃t���O
	bool m_hitEnemeyFlag = false;

};

