#pragma once
#include "AIActor.h"

class Slime :public AIActor
{
public:
	Slime();
	~Slime();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Move();

	Vector3 SetDirection();

	bool IsBumpedForest();

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return true;
	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

	///// <summary>
	///// ��_���[�W�p�����蔻��
	///// </summary>
	//virtual void DamageCollision(CharacterController& characon);
	////�ʏ�U���ɓ����������̏���
	//virtual void HitNormalAttack();
	////�q�[���[�̃X�L���ɓ����������̏���
	//virtual void HitHeroSkillAttack();
	////�E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏���
	//virtual void HitFireBall();
	////�E�B�U�[�h�̃t���C���s���[�ɓ����������̏���
	//virtual void HitFlamePillar();

	bool RotationOnly();

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();


	RigidBody m_rigidBody;		//���́B

	ModelRender m_modelRender;
	CharacterController m_charaCon;

	Vector3 m_direction= Vector3::Zero;

	const float m_angleChangeTime = 5.0f;		//�x�N�g�����v�Z����^�C�}�[

	const float m_distanceToPlayer = 60.0f;

	float m_ramdomAngle = 0.0f;

};

