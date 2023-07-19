#include "stdafx.h"
#include "AIActor.h"

AIActor::AIActor()
{
	m_player = FindGO<Player>("player");
}

AIActor::~AIActor()
{
}

void AIActor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void AIActor::SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Vector3 AIActor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_targetPosition - m_position;
	//���K��
	diff.Normalize();
	diff.y = 0.0f;
	//���x��ݒ�
	moveSpeed = diff * status.defaultSpeed;

	//�l���Z�[�u���Ă���
	m_SaveMoveSpeed = moveSpeed;

	//����̃A�j���[�V�������Đ����̂Ƃ�
	if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}

	
	
}

void AIActor::SetTargetPosition()
{
	//�^�[�Q�b�g(�v���C���[)�̍��W���擾
	m_targetPosition = m_player->GetPosition();
}

bool AIActor::IsFindPlayer(float distance)
{
	//�^�[�Q�b�g(�v���C���[)�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_targetPosition - m_position;
	diff.y = 0.0f;
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


	return false;
}

Quaternion AIActor::Rotation()
{
	if (RotationOnly() == true)
	{
		return m_rotation;
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}

	return m_rotation;
}
