#pragma once

//#include "Wizard.h"


class Lich;
class Wizard;

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

	
	void SetForWizard(const char* collisionname,
		float speed,
		float distance,
		float y_up,
		float collisionradius,
		Vector3 scale
	);

	void SetForLich(
		const char* collisionname, 
		float speed, 
		float distance, 
		float y_up, 
		float collisionradius, 
		Vector3 scale
	);

	/// <summary>
	/// �������̐ݒ�
	/// </summary>
	/// <param name="position">��������L�����̍��W</param>
	/// <param name="rotation">��������L�����̉�]</param>
	void Setting(Vector3 position, Quaternion rotation)
	{
		m_position = position;
		m_rotation = rotation;
	}

	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	void SetLichAtk(int attack)
	{
		m_lichAttack = attack;
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

	Lich* m_lich = nullptr;

	ModelRender m_model;
	CollisionObject* m_BallCollision;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_collisionPosition= Vector3::Zero;
	Vector3 m_forWizardBallScale = Vector3::One;
	Vector3 m_forLichBallScale = { 2.3f,2.3f,2.3f };
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	const char* m_fireBall = "fireball";
	const char* m_darkBall = "darkball";

	//�U����
	int m_atk = 0;

	const int m_wizardAttack = 30;		//�E�B�U�[�h�p�̍U����
	int m_lichAttack = 20;		//���b�`�p�̍U����

	//�t�@�C���[�{�[���̃X�s�[�h
	const float m_speed = 150.0f;



	//�{�[���̐�������
	int m_limitTimer = 0;

	const int m_forWizardLimitTimer = 3;
	const int m_forLichLimitTimer = 5;

	//�^�C�}�[
	float m_moveTime = 0.0f;
	//�v���C���[����{�[���̋���
	const float m_distance = 60.0f;

	//�G�ɓ����������̃t���O
	bool m_hitEnemeyFlag = false;

};

