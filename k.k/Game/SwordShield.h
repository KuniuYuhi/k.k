#pragma once

#include "IWeapon.h"

class SwordShield:public IWeapon
{
public:
	SwordShield();
	~SwordShield();

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
	/// ���̓����蔻�肪�q�b�g������
	/// </summary>
	/// <returns>�q�b�g������true�A�q�b�g���Ȃ�������false</returns>
	bool IsHitShieldCollision();

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
	ModelRender m_modelSword;		//�����f��
	ModelRender m_modelShield;		//�����f��

	CollisionObject* m_swordCollision;	//�Ў茕�̓����蔻��
	CollisionObject* m_shieldCollision;	//���̓����蔻��

	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_shieldPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;
	Matrix m_shieldMatrix = g_matIdentity;

	const float m_jampSpeed = 3000.0f;

	//������������鎞�̃{�[��ID
	int m_armedSwordBoonId = -1;
	int m_armedShieldBoonId = -1;
	//��������܂������̃{�[��ID
	int m_stowedSwordBoonId = -1;
	int m_stowedShieldBoonId = -1;


};

