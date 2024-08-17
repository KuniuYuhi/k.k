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

	//待機フラグがあるなら
	if (m_isWaitingFlag)
	{
		//プレイヤーとの距離より待機する距離が大きいならキャラコンの処理をしないようにする
		if (toPlayerDistance < m_status.GetWaitingDistance())
		{
			isExecute = false;
		}
	}
	else
	{
		//プレイヤーとの距離より接近する距離が大きいならキャラコンの処理をしないようにする
		if (toPlayerDistance < m_status.GetApproachDistance())
		{
			isExecute = false;
		}
	}


	//実行フラグがtrueなら
	if (m_charaCon != nullptr && isExecute)
	{
		//キャラコンで座標を移動する
		m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	}
	

	//回転方向を保存
	m_rotateDirection = m_moveSpeed;
}

void MobEnemyBase::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}
