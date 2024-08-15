#pragma once

#include "WeaponStatus.h"

#include "BraveAnimClipAndStateInfo.h"

class WeaponStatus;
class Brave;

using namespace BraveState;

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
		enArmed,			//�������
		enStowed			//���[���
	};

	/// <summary>
	/// �R���{�X�e�[�g
	/// </summary>
	enum EnComboState
	{
		enCombo_None = static_cast<EnComboState>(enBraveState_FirstAttack - 1),//�E�҂̍U���X�e�[�g�ɒl�����킹��
		enCombo_First = enBraveState_FirstAttack,
		enCombo_Second,
		enCombo_Third,
		enCombo_End,

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
	/// ���݂̃R���{�X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	EnComboState GetCurrentComboState()
	{
		return m_enComboState;
	}



	/// <summary>
	/// ���[��Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeStowedState();

	/// <summary>
	/// ������Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeArmedState();


	/// <summary>
	/// �R���{�U��������i�߂�
	/// </summary>
	virtual void ProceedComboAttack(){}
	/// <summary>
	/// �R���{�U�����������Z�b�g����
	/// </summary>
	virtual void ResetComboAttack(){}

	/// <summary>
	/// ����A�h��A�N�V�������I��邩
	/// </summary>
	/// <returns>�I���Ȃ�true</returns>
	virtual bool IsEndDefensiveAction() { return false; }


protected:

	Brave* m_brave = nullptr;		//�v���C���[�̃C���X�^���X

	WeaponStatus m_status;			//�X�e�[�^�X


	EnWeaponState m_enWeaponState = enStowed;			//����̏�ԁB�f�t�H���g�͎��[���


	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };		//���[��Ԏ��̍��W


	EnComboState m_enComboState = enCombo_None;

};

