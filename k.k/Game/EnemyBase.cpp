#include "stdafx.h"
#include "EnemyBase.h"

void EnemyBase::TakeDamage(int damage)
{
	//ダメージを受ける
	//やられた場合は死亡フラグが立つ
	SetDieFlag(m_status.TakeDamage(damage));
}

float EnemyBase::CalcDistanceToTargetPosition(Vector3 target)
{
	Vector3 diff = target - m_position;
	return diff.Length();
}
