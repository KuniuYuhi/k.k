#include "stdafx.h"
#include "MonsterBase.h"

MonsterBase::MonsterBase()
{
}

MonsterBase::~MonsterBase()
{
}

Vector3 MonsterBase::CalcVelocity(Status status, Vector3 targetposition, bool dashFlag)
{
	Vector3 moveSpeed = Vector3::Zero;
	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = targetposition - m_position;
	diff.y = 0.0f;
	//���K��
	diff.Normalize();
	//�_�b�V���t���O��true�Ȃ�
	if (dashFlag == true)
	{
		//���x��ݒ�
		moveSpeed = diff * (status.defaultSpeed * 1.5f);
	}
	else
	{
		//���x��ݒ�
		moveSpeed = diff * status.defaultSpeed;
	}
	//��΂Ȃ��悤�ɂ���
	moveSpeed.y = 0.0f;
	//�O������ݒ�
	m_forward = diff;
	//�l���Z�[�u���Ă���
	m_SaveMoveSpeed = moveSpeed;
	return moveSpeed;
}

bool MonsterBase::IsInFieldOfView(Vector3 toPlayerDir, Vector3 forward, float angle)
{
	//�x�N�g�����K��
	toPlayerDir.Normalize();
	//�^�[�Q�b�g�Ɍ������x�N�g���ƑO�����̓��ς��v�Z����
	float t = toPlayerDir.Dot(forward);
	//���ς̌��ʂ�acos�֐��ɓn���āAm_enemyFowrad��toPlayerDir�̂Ȃ��p�x�����߂�B
	float calcangle = acos(t);
	//����p����B�w�肵���p�x�͈͓̔��̒l��
	if (fabsf(calcangle) < Math::DegToRad(angle))
	{
		//�͈͓��Ȃ�
		return true;
	}
	//�͈͊O�Ȃ�
	return false;
}

bool MonsterBase::IsFindPlayer(float distance)
{
	//�^�[�Q�b�g(�v���C���[)�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_targetPosition - m_position;
	diff.y = 0.0f;
	//�x�N�g�����Z�[�u
	m_toTarget = diff;
	//�v�Z�����x�N�g��������̋�����菬����������
	if (diff.Length() < distance)
	{
		//������
		return true;
	}
	else
	{
		//������Ȃ�����
		m_targetPosition = Vector3::Zero;
		return false;
	}
}

bool MonsterBase::AttackInterval(const float attackintarvaltime)
{
	//�U��������
	if (m_attackFlag == true)
	{
		//�^�C�}�[���C���^�[�o���𒴂�����
		if (attackintarvaltime < m_attackIntervalTimer)
		{
			//�U���\�ɂ���
			m_attackFlag = false;
			//�^�C�}�[�����Z�b�g
			m_attackIntervalTimer = 0.0f;
		}
		else
		{
			m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//�U���s�\
			return false;
		}
	}
	//�U���\
	return true;
}

bool MonsterBase::AngleChangeTimeIntarval(float LimitTime)
{
	return false;
}

void MonsterBase::CreateDamageFont(int damage)
{
}

bool MonsterBase::IsCollisionDetection()
{
	//����̃A�j���[�V�������Đ����Ȃ�
	if (isAnimationEntable() != true)
	{
		//��������Ȃ�
		return false;
	}
	//������Ƃ�
	return true;
}

Quaternion MonsterBase::Rotation(float rotSpeed, float rotOnlySpeed)
{
	if (RotationOnly() == true)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
			m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		}
		//�O������ݒ�
		/*m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);*/
		m_forward = m_rotMove;
		m_forward.Normalize();
		return m_rotation;
	}

	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ɂ₩�ɉ�]������
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	}
	//�O������ݒ�
	/*m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);*/
	m_forward = m_rotMove;
	m_forward.Normalize();
	return m_rotation;
}
