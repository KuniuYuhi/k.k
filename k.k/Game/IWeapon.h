#pragma once

class Brave;

/// <summary>
/// �C���^�[�t�F�[�X�̎���
/// </summary>
class IWeapon:public IGameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	IWeapon(){}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IWeapon(){}

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


};

