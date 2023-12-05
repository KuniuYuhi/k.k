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
	/// �{�X�̃X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	Status GetStatus()
	{
		return m_bossBase->GetStatus();
	}

private:
	/// <summary>
	/// �{�X�𐶐�
	/// </summary>
	void CreatBossCharacter();


private:
	BossBase* m_bossBase = nullptr;

	Summoner* m_sumoner = nullptr;
	Lich* m_lich = nullptr;

	Vector3 m_position = g_vec3Zero;


};

