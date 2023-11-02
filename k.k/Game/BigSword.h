#pragma once

#include "IWeapon.h"

class BigSword:public IWeapon
{
public:
	BigSword();
	~BigSword();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// �U����X�L�����̃W�����v�̑��x�̎擾
	/// </summary>
	/// <returns></returns>
	float GetJampSpeed()
	{
		return m_jampSpeed;
	}

	/// <summary>
	/// �X�L���U������
	/// </summary>
	void ProcessSkillAttack();

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// �����蔻��̐���
	/// </summary>
	void InitCollision() override;
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;

private:
	ModelRender m_modelBigSword;		//�����f��

	CollisionObject* m_bigSwordCollision;	//���茕�̓����蔻��

	Vector3 m_swordPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	//������������鎞�̃{�[��ID
	int m_armedSwordBoonId = -1;

	const float m_jampSpeed = 30000.0f;

};

