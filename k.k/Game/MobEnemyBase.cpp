#include "stdafx.h"
#include "MobEnemyBase.h"

#include "MobEnemyMovement.h"




float MobEnemyBase::CalcDistanceToTargetPosition(Vector3 target)
{
	Vector3 diff = target - m_position;
	return diff.Length();
}

void MobEnemyBase::SettingDefaultComponent()
{
	AddComponent<MobEnemyMovement>();
	m_movement = GetComponent<MobEnemyMovement>();

}

void MobEnemyBase::ChaseMovement(Vector3 targetPosition)
{

	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed
	);

	float toPlayerDistance = CalcDistanceToTargetPosition(targetPosition);
	bool isExecute = true;

	//�ҋ@�t���O������Ȃ�
	if (m_isWaitingFlag)
	{
		//�v���C���[�Ƃ̋������ҋ@���鋗�����傫���Ȃ�L�����R���̏��������Ȃ��悤�ɂ���
		if (toPlayerDistance < m_status.GetWaitingDistance())
		{
			isExecute = false;
		}
	}
	else
	{
		//�v���C���[�Ƃ̋������ڋ߂��鋗�����傫���Ȃ�L�����R���̏��������Ȃ��悤�ɂ���
		if (toPlayerDistance < m_status.GetApproachDistance())
		{
			isExecute = false;
		}
	}


	//���s�t���O��true�Ȃ�
	if (m_charaCon != nullptr && isExecute)
	{
		//�L�����R���ō��W���ړ�����
		m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	}
	

	//��]������ۑ�
	m_rotateDirection = m_moveSpeed;
}

void MobEnemyBase::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}
