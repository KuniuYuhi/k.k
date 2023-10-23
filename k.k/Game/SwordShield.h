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

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;

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

