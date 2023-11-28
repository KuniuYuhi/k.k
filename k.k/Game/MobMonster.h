#pragma once
#include "MonsterBase.h"
#include "KnockBack.h"

class Lich;

class MobMonster:public MonsterBase
{
public:

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="charaCon">�ړ����������I�u�W�F�N�g�̃L�����N�^�[�R���g���[���[</param>
	void Move(CharacterController& charaCon);

	/// <summary>
	/// �������x�N�g����ݒ�
	/// </summary>
	/// <param name="range"></param>
	/// <returns></returns>
	Vector3 SetRamdomDirection(int range);

	/// <summary>
	/// ��(�X)�ɂԂ�������
	/// </summary>
	/// <returns></returns>
	virtual bool IsBumpedForest(float pos2Length);

	/// <summary>
	/// �����X�^�[�ɂԂ�������
	/// </summary>
	/// <returns></returns>
	bool IsBumpedMonster();

	/// <summary>
	/// �͂����p���[������������
	/// </summary>
	void SubPassPower();

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const = 0;

	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEnable() const = 0;

	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEnable() const = 0;

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// ���ꂽ�Ƃ��̏���
	/// </summary>
	/// <param name="seFlag">���ꂽ�Ƃ��̌��ʉ����Đ����邩�̃t���O</param>
	virtual void ProcessDead(bool seFlag = true);

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="rotSpeed">��]���鑬�x(�ړ�)</param>
	/// <param name="rotOnlySpeed">��]���鑬�x(�ړ����Ȃ���)</param>
	/// <returns></returns>
	//Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// �q�b�g�G�t�F�N�g����
	/// </summary>
	void CreateHitEffect();

	/// <summary>
	/// �m�b�N�o�b�N�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetKnockBackFlag(bool flag)
	{
		m_knockBackFlag = flag;
	}
	/// <summary>
	/// �m�b�N�o�b�N�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetKnockBackFlag() const
	{
		return m_knockBackFlag;
	}

	/// <summary>
	/// ���b�`�̃C���X�^���X����
	/// </summary>
	/// <param name="lich"></param>
	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

protected:

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() = 0;

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// ��]�݂̂��s����������
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly();


	Lich*					m_lich = nullptr;

	Vector3					m_passPower = g_vec3One;		//�͂����Ƃ��̗�

	Vector3					m_direction = Vector3::Zero;	//�ړ���������x�N�g��

	float					m_angleChangeTime = 0.0f;		//�������x�N�g�����v�Z����^�C�}�[

	float					m_distanceToPlayer = 0.0f;		//�v���C���[�Ƃ̋���
	float					m_attackRange = 0.0f;			//�U���ł���͈�
	float					m_stayRange = 0.0f;				//�ړ����Ȃ��͈�

	float					m_ramdomAngle = 0.0f;			//�����_���ȃA���O���B�ړ����������߂�Ƃ��Ɏg��

	float					m_attackIntervalTime = 0.0f;	//�U��������̃C���^�[�o��

	int						m_angleRange = 2;				//�ړ�����A���O���͈̔�

	float					m_pos2Length = 0.0;				//�ǂƔ�

	bool                    m_chasePlayerFlag = false;		//�v���C���[��ǂ������邩�̃t���O

	bool					m_knockBackFlag = false;		//�m�b�N�o�b�N���邩�̃t���O

	float					m_knockBackTimer = 0.0f;
};

