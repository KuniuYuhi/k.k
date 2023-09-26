#pragma once

/// <summary>
/// �}�W�b�N�{�[���N���X�B�`�{�[���̊��N���X
/// </summary>
class MagicBall:public IGameObject
{
public:
	MagicBall();
	virtual ~MagicBall();

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
	/// �ړ������̐ݒ�
	/// </summary>
	void SetMoveSpeed()
	{
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
	}

	virtual void Explosion();


	/// <summary>
	/// �G�ɓ����������̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetHitFlag(bool flag)
	{
		m_hitFlag = flag;
	}

	/// <summary>
	/// �h���N���X�̍U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	virtual const int& GetBallAtk() const = 0;

	/// <summary>
	/// �U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return GetBallAtk();
	}


protected:

	Vector3						m_position = Vector3::Zero;
	Vector3						m_moveSpeed = Vector3::Zero;
	Quaternion					m_rotation = Quaternion::Identity;

	float						m_speed = 0.0f;

	float						m_moveTimer = 0.0f;		//�{�[�����������Ԃ��v�Z����^�C�}�[

	bool						m_hitFlag = false;			//�G�ɓ����������̃t���O

};

