#pragma once

#include "WeaponStatus.h"

class WeaponStatus;
class Brave;

/// <summary>
/// ����̊��N���X
/// </summary>
class WeaponBase:public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	WeaponBase(Brave* brave)
	{
		m_brave = brave;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~WeaponBase() = default;




	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack() = 0;


protected:

	Brave* m_brave = nullptr;		//�v���C���[�̃C���X�^���X

	WeaponStatus m_status;			//�X�e�[�^�X




};

