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
	/// �f�X�g���N�^
	/// </summary>
	virtual ~WeaponBase() = default;

	/// <summary>
	/// ����̏��
	/// </summary>
	enum EnWeaponState
	{
		enArmed,
		enStowed
	};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �U������
	/// </summary>
	virtual void AttackAction() = 0;



	/// <summary>
	/// ���݂̕���̏�Ԃ�ݒ�
	/// </summary>
	/// <param name="setWeaponState"></param>
	void SetCurrentWeaponState(EnWeaponState setWeaponState)
	{
		m_enWeaponState = setWeaponState;
	}

	/// <summary>
	/// ���݂̕���̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	EnWeaponState GetCurrentWeaponState()
	{
		return m_enWeaponState;
	}


	/// <summary>
	/// ���[��Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeStowedState();

	/// <summary>
	/// ������Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeArmedState();



protected:

	Brave* m_brave = nullptr;		//�v���C���[�̃C���X�^���X

	WeaponStatus m_status;			//�X�e�[�^�X


	EnWeaponState m_enWeaponState = enStowed;			//����̏�ԁB�f�t�H���g�͎��[���


	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };		//���[��Ԏ��̍��W




};

