#pragma once
#include "IAttackObject.h"

/// <summary>
/// �}�W�b�N�{�[���N���X�B�`�{�[���̊��N���X
/// </summary>
class MagicBall:public IAttackObject
{
public:
	MagicBall();
	virtual ~MagicBall();

	/// <summary>
	/// �������̐ݒ�
	/// </summary>
	/// <param name="position">��������L�����̍��W</param>
	/// <param name="rotation">��������L�����̉�]</param>
	void Setting(Vector3 position, Quaternion rotation)
	{
		m_position = position;
		m_rotation = rotation;
	}

	/// <summary>
	/// �����J�n���W��ݒ�
	/// </summary>
	/// <param name="startPosition"></param>
	void SetStartPosition(Vector3 startPosition)
	{
		m_startPosition = startPosition;
	}

	/// <summary>
	/// �ړ������̐ݒ�
	/// </summary>
	void SetMoveSpeed()
	{
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
	}

	/// <summary>
	/// �G�ɓ����������̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetHitFlag(bool flag)
	{
		m_hitFlag = flag;
	}

protected:
	/// <summary>
	/// ��������
	/// </summary>
	virtual void Explosion();

	/// <summary>
	/// ���i�ړ�����
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// �ǔ��ړ�����
	/// </summary>
	/// <param name="targetPosition">�ǔ��Ώۂ̍��W</param>
	void MoveChase(Vector3 targetPosition);

	/// <summary>
	/// �ړ��\���ԃ^�C�}�[�̌v�Z
	/// </summary>
	/// <param name="moveLimitTime">��������</param>
	void CalcMoveTime(const float moveLimitTime);

	/// <summary>
	/// �����I�ɍ폜���邩�B�ړ������ō폜
	/// </summary>
	/// <param name="currentPosition">���݂̍��W</param>
	/// <param name="deleteLength">�폜���鋗���̒���</param>
	/// <returns>true�ō폜�\</returns>
	bool IsForceDelete(Vector3 currentPosition, float deleteLength);

protected:

	Vector3						m_startPosition = g_vec3Zero;
	Vector3						m_position = g_vec3Zero;
	Vector3						m_moveSpeed = g_vec3Zero;
	Quaternion					m_rotation = g_quatIdentity;



	float						m_speed = 0.0f;

	float						m_moveTimer = 0.0f;		//�{�[�����������Ԃ��v�Z����^�C�}�[

	bool						m_hitFlag = false;			//�G�ɓ����������̃t���O

};

