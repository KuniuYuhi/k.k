#pragma once

#include "WeaponBase.h"

class SwordShield:public WeaponBase
{
public:
	SwordShield();
	~SwordShield();

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
	float GetJampSpeed() override
	{
		return m_jampSpeed;
	}

	/// <summary>
	/// ���̓����蔻�肪�q�b�g������
	/// </summary>
	/// <returns>�q�b�g������true�A�q�b�g���Ȃ�������false</returns>
	bool IsHitCollision() override;

	/// <summary>
	/// �X�L���U������
	/// </summary>
	void ProcessSkillAttack() override;
	
	/// <summary>
	/// �ϋv�l���Ȃ��Ȃ�����Ƃ��̏���
	/// </summary>
	void ProcessNoEndurance() override
	{
		//���̑ϋv�l���Ȃ��Ȃ����̂ŁA�h��s�\�ɂ���
		SetIsDefendEnableFlag(false);
	}
	/// <summary>
	/// �ϋv�l��0���傫���Ȃ����Ƃ��̏���(0����ȏ�ɂȂ�����)
	/// </summary>
	void ProcessOnEndurance() override
	{
		//���̑ϋv�l���������̂ŁA�h��\�ɂ���
		SetIsDefendEnableFlag(true);
	}

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
	/// ���̓����蔻��̏�����
	/// </summary>
	void InitSwordCollision();
	/// <summary>
	/// ���̓����蔻��̏�����
	/// </summary>
	void InitShieldCollision();
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

private:
	ModelRender m_modelSword;		//�����f��
	ModelRender m_modelShield;		//�����f��

	CollisionObject* m_swordCollision = nullptr;	//�Ў茕�̓����蔻��
	CollisionObject* m_shieldCollision = nullptr;	//���̓����蔻��

	Vector3 m_skillMovePos = g_vec3Zero;	//�X�L�����g���Ƃ��̍��W
	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_shieldPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;
	Matrix m_shieldMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	Quaternion rot;

	const float m_jampSpeed = 3000.0f;

	//������������鎞�̃{�[��ID
	int m_armedSwordBoonId = -1;
	int m_armedShieldBoonId = -1;
	//��������܂������̃{�[��ID
	int m_stowedSwordBoonId = -1;
	int m_stowedShieldBoonId = -1;

	const float m_knockBackPower = 200.0f;	//�m�b�N�o�b�N�p���[

};

