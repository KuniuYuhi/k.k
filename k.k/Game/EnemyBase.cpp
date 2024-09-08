#include "stdafx.h"
#include "EnemyBase.h"

void EnemyBase::TakeDamage(int damage)
{
	//�_���[�W���󂯂�
	//���ꂽ�ꍇ�͎��S�t���O������
	SetDieFlag(m_status.TakeDamage(damage));
}

float EnemyBase::CalcDistanceToTargetPosition(Vector3 target)
{
	Vector3 diff = target - m_position;
	return diff.Length();
}
