#pragma once

class Brave;

class IWeapon:public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	IWeapon();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IWeapon();

	/// <summary>
	/// ����̏�ԃX�e�[�g
	/// </summary>
	enum EnWeaponState
	{
		enWeaponState_Stowed,		//���[���Ă�����
		enWeaponState_Armed,		//�������Ă�����
		enWeaponState_None,			//�����������Ă��Ȃ�
	};
	/// <summary>
	/// ����̖h��̃^�C�v�B
	/// </summary>
	enum EnDefendTipe
	{
		enDefendTipe_Defence,	//���ȂǂŖh���^�C�v
		enDefendTipe_avoid,		//����^�C�v
		enDefendTipe_None
	};

	////////////////////////////////////////////////////////////////////////
	///���z�֐��A�������z�֐�
	////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() = 0;
	/// <summary>
	/// �����蔻��̐���
	/// </summary>
	virtual void InitCollision() = 0;
	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	virtual void MoveWeapon() = 0;
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	virtual void MoveArmed() = 0;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	virtual void MoveStowed() = 0;

	//todo ����ŗL�̏���

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
	/*/// <summary>
	/// ����̏�ԃX�e�[�g��Ԃ�
	/// </summary>
	/// <returns>�u0�v�Ŏ��[��ԁA�u1�v�ő�����ԁA�u2�v�ŉ����������Ă��Ȃ����</returns>
	const int& GetWeaponState() const
	{
		return m_enWeaponState;
	}*/

	/// <summary>
	/// ����̍U���͂̐ݒ�
	/// </summary>
	/// <param name="power"></param>
	void SetPower(int power)
	{
		m_power = power;
	}
	/// <summary>
	/// ����̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	const int& GetPower() const
	{
		return m_power;
	}

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
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

protected:

	/// <summary>
	/// ����̖h��^�C�v���擾
	/// </summary>
	/// <returns></returns>
	void SetEnDefendTipe(EnDefendTipe defendTipe)
	{
		m_enDefendTipe = defendTipe;
	}

protected:
	Brave* m_brave = nullptr;

	EnWeaponState m_enWeaponState = enWeaponState_None;

	EnDefendTipe m_enDefendTipe = enDefendTipe_None;

	int m_power = 0;		//����̍U����

	Vector3 m_position = g_vec3Zero;	//���g�̍��W

	bool m_stowedFlag = false;		//���[��ԃt���O�B�������Ȃ��悤�ɂ��邽�߂̃t���O


	//�h���N���X�ŕK�v�Ȃ���
	//���[���h���W
	//�������������{�[��ID
	//��������܂������̃{�[��ID
};

