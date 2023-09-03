#pragma once
#include "AIActor.h"

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
	bool IsBumpedForest();

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



	Vector3 m_direction = Vector3::Zero;

	float					m_angleChangeTime = 0.0f;					//�������x�N�g�����v�Z����^�C�}�[

	const float				m_distanceToPlayer = 0.0f;				//�v���C���[�Ƃ̋���
	const float				m_attackRange = 0.0f;						//�U���ł���͈�
	const float				m_stayDistance = 0.0f;						//�ړ����Ȃ�����

	float					m_ramdomAngle = 0.0f;						//

	const float				m_attackIntervalTime = 0.0f;				//�U��������̃C���^�[�o��

	const int				m_angleRange = 2;					//�ړ�����A���O���͈̔�
};

