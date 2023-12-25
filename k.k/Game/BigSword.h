#pragma once

#include "WeaponBase.h"

class BigSword:public WeaponBase
{
public:
	BigSword();
	~BigSword();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

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
	void InitCollision();
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;

	/// <summary>
	/// �㏸����
	/// </summary>
	void ProcessRising();

	/// <summary>
	/// ���~����
	/// </summary>
	void ProcessFall();

	/// <summary>
	/// �n�ʂƂ̏Փ˔���
	/// </summary>
	bool IsGround();

private:
	ModelRender m_modelBigSword;		//�����f��

	CollisionObject* m_bigSwordCollision = nullptr;	//���茕�̓����蔻��

	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_maxRisingPosition = g_vec3Zero;

	Vector3 m_skillMovePos = g_vec3Zero;	//�X�L�����g���Ƃ��̍��W

	Matrix m_swordMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	//������������鎞�̃{�[��ID
	int m_armedSwordBoonId = -1;

	const float m_jampSpeed = 30000.0f;

	const float m_knockBackPower = 300.0f;

};

