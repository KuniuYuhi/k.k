#pragma once
#include "AIActor.h"

class Lich;

class MobMonster:public AIActor
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
	Vector3 SetDirection(int range);

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
	virtual bool isAnimationEntable() const = 0;

	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const = 0;

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// ���ꂽ�Ƃ��̏���
	/// </summary>
	virtual void Dead();

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="rotSpeed">��]���鑬�x(�ړ�)</param>
	/// <param name="rotOnlySpeed">��]���鑬�x(�ړ����Ȃ���)</param>
	/// <returns></returns>
	Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// �q�b�g�G�t�F�N�g����
	/// </summary>
	void CreateHitEffect();

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

	Vector3					m_direction = Vector3::Zero;

	float					m_angleChangeTime = 0.0f;		//�������x�N�g�����v�Z����^�C�}�[

	float					m_distanceToPlayer = 0.0f;		//�v���C���[�Ƃ̋���
	float					m_attackRange = 0.0f;			//�U���ł���͈�
	float					m_stayRange = 0.0f;				//�ړ����Ȃ��͈�

	float					m_ramdomAngle = 0.0f;			//�����_���ȃA���O���B�ړ����������߂�Ƃ��Ɏg��

	float					m_attackIntervalTime = 0.0f;	//�U��������̃C���^�[�o��

	int						m_angleRange = 2;				//�ړ�����A���O���͈̔�

	float					m_pos2Length = 0.0;				//�ǂƔ�
};

