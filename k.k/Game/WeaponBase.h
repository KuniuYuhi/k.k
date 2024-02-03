#pragma once
#include "IWeapon.h"
#include "HitDetection.h"
#include "Status.h"
#include "InitEffect.h"

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

	/// <summary>
	/// ��_���[�W�ȂǂŁA����ɖ߂��͂��������ϐ��������I�Ƀ��Z�b�g����
	/// </summary>
	virtual void ResetVariable() {}

	/// <summary>
	/// �ϋv�l���Ȃ��Ȃ�����Ƃ��̏���
	/// </summary>
	virtual void ProcessNoEndurance(){}

	/// <summary>
	/// �ϋv�l��0���傫���Ȃ����Ƃ��̏���(0����ȏ�ɂȂ�����)
	/// </summary>
	virtual void ProcessOnEndurance() {}

	/// <summary>
	/// ��_���[�W��̒l�̃��Z�b�g�B�X�L���Ȃǂ����f���ꂽ�Ƃ��p
	/// </summary>
	virtual void postDamageReset(){}

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
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus() const
	{
		return m_status;
	}

	/// <summary>
	/// �ϋv�l�̑����̌v�Z�B
	/// </summary>
	/// <param name="value"></param>
	/// <param name="addOrSubFlag"></param>
	void CalcEndurance(int value, bool addOrSubFlag);

	/// <summary>
	/// �h��\���t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDefendEnableFlag(bool flag)
	{
		m_isDefendEnableFlag = flag;
	}
	/// <summary>
	/// �h��\���t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDefendEnableFlag() const
	{
		return m_isDefendEnableFlag;
	}
	/// <summary>
	/// �U���\���̃t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsAttackEnableFlag(bool flag)
	{
		m_isAttackEnableFlag = flag;
	}
	/// <summary>
	/// �U���\���̃t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsAttackEnableFlag() const
	{
		return m_isAttackEnableFlag;
	}

	/// <summary>
	/// �ϋv�l��0���傫�����ǂ�������
	/// </summary>
	/// <returns>true�ōU���\�Afalse�ōU���s�\</returns>
	const bool& IsWeaponEndurance() const
	{
		if (m_status.GetEndurance() > 0)
		{
			return true;
		}
		return false;
	}

	/// <summary>
	/// �O�i����X�s�[�h��ݒ�
	/// </summary>
	/// <param name="Speed"></param>
	void SetMoveForwardSpeed(float Speed)
	{
		m_moveForwardSpeed = Speed;
	}
	/// <summary>
	/// �O�i����X�s�[�h���擾 
	/// </summary>
	/// <returns></returns>
	const float& GetMoveForwardSpeed() const
	{
		return m_moveForwardSpeed;
	}

	/// <summary>
	/// ��ނ���X�s�[�h��ݒ�
	/// </summary>
	/// <param name="Speed"></param>
	void SetMoveBackSpeed(float Speed)
	{
		m_moveForwardSpeed = Speed;
	}
	/// <summary>
	/// ��ނ���X�s�[�h���擾
	/// </summary>
	/// <returns></returns>
	const float& GetMoveBackSpeed() const
	{
		return m_moveForwardSpeed;
	}

	/// <summary>
	/// �m�b�N�o�b�N�p���[�̎擾
	/// </summary>
	/// <param name="num">�擾�������p���[�̔ԍ�</param>
	/// <returns></returns>
	const float& GetKnockBackPower(int num);


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

	void PlayEffect(
		InitEffect::EnEFK enEffect,
		Vector3 position,
		float size,
		Quaternion rotation = g_quatIdentity
	);
	

protected:

	enum EnKnockBackPowerNumber
	{
		enKnockBackPowerNumber_None,
		enKnockBackPowerNumber_1combo,
		enKnockBackPowerNumber_2combo,
		enKnockBackPowerNumber_3combo,
		enKnockBackPowerNumber_Skill,
	};

	Status              m_status;
	Brave*				m_brave = nullptr;

	HitDetection		m_hitDelection;

	EnWeaponState		m_enWeaponState = enWeaponState_None;

	EnDefendTipe		m_enDefendTipe = enDefendTipe_None;

	int					m_power = 0;							//����̍U����

	bool				m_stowedFlag = false;					//���[��ԃt���O�B�������Ȃ��悤�ɂ��邽�߂̃t���O

	bool				m_RotationDelectionFlag = false;		//��]�\�t���O

	bool				m_isDefendEnableFlag = true;			//�h��\���̃t���O�Btrue�ŉ\

	bool				m_isAttackEnableFlag = true;			//�U���\���̃t���O�Btrue�ŉ\

	float				m_moveForwardSpeed = 2.0f;				//�O�i����Ƃ��̃X�s�[�h

	float				m_moveBackSpeed = 1.0f;					//��ނ���Ƃ��̃X�s�[�h

	float m_knockBackSpeed = 1.0f;
};

