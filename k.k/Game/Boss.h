#pragma once

#include "Summoner.h"


class BossBase;
class Summoner;
class Lich;

/// <summary>
/// �{�X�𐶐��A���䂷��N���X
/// </summary>
class Boss:public IGameObject
{
public:
	Boss();
	~Boss();

	bool Start();
	void Update();
	
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetBossPosition(Vector3 position)
	{
		m_bossBase->SetPosition(position);
	}
	/// <summary>
	/// �{�X���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_bossBase->GetPosition();
	}
	/// <summary>
	/// �O�����̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_bossBase->GetForward();
	}

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const
	{
		return m_bossBase->GetRotation();
	}

	/// <summary>
	/// �{�X�̃X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	Status GetStatus()
	{
		return m_bossBase->GetStatus();
	}

	/// <summary>
	/// �L�����N�^�[�R���g���[���[�̎擾
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharacterController()
	{
		return m_bossBase->GetCharacterController();
	}


	/// <summary>
	/// �_���[�W���󂯂��邩�̃t���O��ݒ�
	/// </summary>
	/// <returns></returns>
	void SetDamageHitEnableFlag(bool flag)
	{
		m_bossBase->SetDamageHitEnableFlag(flag);
	}
	/// <summary>
	/// �_���[�W���󂯂��邩�̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetDamageHitEnableFlag() const
	{
		return m_bossBase->GetDamageHitEnableFlag();
	}



private:
	/// <summary>
	/// �{�X�𐶐�
	/// </summary>
	void CreatBossCharacter();

	/// <summary>
	/// �{�X���폜���Ă��悢��
	/// </summary>
	void IsDeleteBoss();

private:
	BossBase* m_bossBase = nullptr;

	Summoner* m_sumoner = nullptr;
	Lich* m_lich = nullptr;

	Vector3 m_position = g_vec3Zero;


};

