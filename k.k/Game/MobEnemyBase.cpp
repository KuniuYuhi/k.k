#include "stdafx.h"
#include "MobEnemyBase.h"

#include "MobEnemyMovement.h"




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

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());


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
