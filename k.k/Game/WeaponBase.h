#pragma once
#include "IWeapon.h"
#include "HitDetection.h"

class Brave;
class HitDetection;

class WeaponBase:public IWeapon
{
public:
	WeaponBase(){}
	virtual ~WeaponBase(){}

	////////////////////////////////////////////////////////////////////////
	///���z�֐��A�������z�֐�
	////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() override{}
	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	virtual void MoveWeapon() override{}
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	virtual void MoveArmed() override{}
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	virtual void MoveStowed() override{}
	/// <summary>
	/// ����̓����蔻��
	/// </summary>
	/// <returns></returns>
	virtual bool IsHitCollision()
	{
		return false;
	}
	/// <summary>
	/// �X�L���U������
	/// </summary>
	virtual void ProcessSkillAttack(){}

	/// <summary>
	/// �U����X�L�����̃W�����v�̑��x�̎擾
	/// </summary>
	/// <returns></returns>
	virtual float GetJampSpeed()
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////
	///���̑��̊֐�
	///////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �o�g���J�n���̕���̏�Ԃ̐ݒ�
	/// </summary>
	/// <param name="enWeaponState"></param>
	void StartSetWeaponState(EnWeaponState enWeaponState);

	/// <summary>
	/// ����̏�ԃX�e�[�g���t�̏�Ԃɕς���B
	/// </summary>
	/// <param name="NowWeaponState">���݂̕���̏�ԃX�e�[�g</param>
	void ReverseWeaponState();
	/// <summary>
	/// ����̏�ԃX�e�[�g��ݒ�
	/// </summary>
	/// <param name="nextWeaponState"></param>
	void SetWeaponState(EnWeaponState enWeaponState)
	{
		m_enWeaponState = enWeaponState;
	}
	/// <summary>
	/// ����̏�ԃX�e�[�g��Ԃ�
	/// </summary>
	/// <returns></returns>
	EnWeaponState& GetWeaponState()
	{
		return m_enWeaponState;
	}

	/// <summary>
	/// ����̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	const int& GetWeaponPower() const
	{
		return m_power;
	}

	/// <summary>
	/// ���[��ԃt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetStowedFlag(bool flag)
	{
		m_stowedFlag = flag;
	}
	/// <summary>
	/// ���[��ԃt���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetStowedFlag() const
	{
		return m_stowedFlag;
	}

	/// <summary>
	/// ����̖h��^�C�v���擾
	/// </summary>
	/// <returns></returns>
	const EnDefendTipe& GetEnDefendTipe() const
	{
		return m_enDefendTipe;
	}
	/// <summary>
	/// ���i�q�b�g�U�����q�b�g�������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetHittableFlag(bool flag)
	{
		m_hitDelection.SetHittableFlag(flag);
	}
	/// <summary>
	/// ���i�q�b�g�U�����q�b�g�������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetHittableFlag() const
	{
		return m_hitDelection.GetHittableFlag();
	}

	/// <summary>
	/// ��]�\�t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetRotationDelectionFlag(bool flag)
	{
		m_RotationDelectionFlag = flag;
	}
	/// <summary>
	/// ��]�\�t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetRotationDelectionFlag() const
	{
		return m_RotationDelectionFlag;
	}

	/// <summary>
	/// ��_���[�W�ȂǂŁA����ɖ߂��͂��������ϐ��������I�Ƀ��Z�b�g����
	/// </summary>
	virtual void ResetVariable(){}

protected:
	/// <summary>
	/// ����̖h��^�C�v��ݒ�
	/// </summary>
	/// <returns></returns>
	void SetEnDefendTipe(EnDefendTipe defendTipe)
	{
		m_enDefendTipe = defendTipe;
	}
	/// <summary>
	/// ����̍U���͂�ݒ�
	/// </summary>
	/// <param name="power"></param>
	void SetWeaponPower(int power)
	{
		m_power = power;
	}

protected:

	Brave*				m_brave = nullptr;

	HitDetection		m_hitDelection;

	EnWeaponState		m_enWeaponState = enWeaponState_None;

	EnDefendTipe		m_enDefendTipe = enDefendTipe_None;

	int					m_power = 0;							//����̍U����

	bool				m_stowedFlag = false;					//���[��ԃt���O�B�������Ȃ��悤�ɂ��邽�߂̃t���O

	bool				m_RotationDelectionFlag = false;		//��]�\�t���O

};

