#include "stdafx.h"
#include "BossEnemyBase.h"
#include "DamageProvider.h"
#include "BossEnemyMovement.h"

#include "Brave.h"

#include "DamageFont.h"

#include "KnockBackInfoManager.h"




void BossEnemyBase::SetDamageInfo(
	int damageId, int attackPower, float knockBackTimeScale,
	EnKnockBackPattern knockBackPattern,
	EnWeaponAttribute weaponAttribute)
{
	m_damageProvider->SetDamageInfo(damageId, attackPower, knockBackTimeScale,
		knockBackPattern, weaponAttribute);
}

void BossEnemyBase::SettingDefaultComponent()
{
	//�{�X�̈ړ��p�R���|�[�l���g
	AddComponent<BossEnemyMovement>();
	m_movement = GetComponent<BossEnemyMovement>();

	//�_���[�W�v���o�C�_�[�R���|�[�l���g
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
	//���g�̃C���X�^���X��ۑ�
	m_damageProvider->SetProviderCharacterInstance(this);


}

void BossEnemyBase::CheckSelfCollision()
{
	//��_���[�W���󂯂Ȃ�����
	if (IgnoreCollision()) return;


	//�v���C���[�̍U���R���W�������擾
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_playerAttackCollisionName
		);

	//�R���W�����̔z���for���ŉ�
	for (auto collision : Collisions)
	{
		//�����蔻�肪�L���łȂ��Ȃ��΂�
		if (!collision->IsEnable()) continue;

		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(*m_charaCon) == true)
		{
			//�R���W�����������Ă���L�����̃_���[�W�v���o�C�_�[�R���|�[�l���g���擾
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr) return;

			//�U�����q�b�g����
			dp->Hit();

			//�U��ID���擾
			int currentAttackId = dp->GetAttackId();

			//�O���ID�������Ȃ珈�����Ȃ�
			if (currentAttackId == oldAttackId) return;

			//�_���[�W�������_���ɒ������āA�t�H���g�Ƃ��ĕ\��
			CreateDamageFont(dp->GetAdjustedDamage(), DamageFont::enDamageActor_Boss);

			//��_���[�W�������s�������ɏ�������
			ProcessHit(dp->GetProviderDamageInfo());

			//�����ID��O�t���[����ID�ɕۑ�
			oldAttackId = currentAttackId;
		}
	}
}

void BossEnemyBase::ChaseMovement(Vector3 targetPosition)
{
	//�A�N�V�������͈ړ��������Ȃ�
	if (IsAction()) return;

	
	//�ړ����x���v�Z
	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed
	);

	//�v���C���[�Ɍ������������v�Z
	float toPlayerDistance = CalcDistanceToTargetPosition(targetPosition);
	bool isExecute = true;
	Vector3 tempMoveSpeed = m_moveSpeed;

	//todo �󋵂ɂ���Ď~�܂鋗���ύX����̂�����

	//�ړ�����߂�Ȃ�
	if (m_isStopMove)
	{
		isExecute = false;
		m_moveSpeed = g_vec3Zero;
	}
	//�v���C���[�Ƃ̋������ڋ߂��鋗�����傫���Ƃ��A����̋����������L�΂�
	else if (toPlayerDistance <= m_status.GetApproachDistance())
	{
		isExecute = false;
		m_moveSpeed = g_vec3Zero;
		//�ڋ߂ł��鋗���������L�΂�
		if (m_isArrivedApproachDistance)
		{
			m_status.AddApproachDistance(200.0f);
		}
	}


	//���s�t���O��true�Ȃ�
	if (m_charaCon != nullptr && isExecute)
	{
		//�d�͂̌v�Z
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
		//�L�����R���ō��W���ړ�����
		m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		if (m_charaCon.get()->IsOnGround()) {
			//�n�ʂɂ����B
			m_moveSpeed.y = 0.0f;
		}
	}

	//�ړ��ʂ�����ΑO������ݒ�
	if (fabsf(tempMoveSpeed.x) >= 0.001f || fabsf(tempMoveSpeed.z) >= 0.001f)
	{
		SetForward(tempMoveSpeed);
	}

	//��]������ۑ�
	m_rotateDirection = tempMoveSpeed;

}

void BossEnemyBase::TurnToTarget()
{
	if (m_player == nullptr) return;

	Vector3 direction = g_vec3Zero;
	//�v���C���[�̕��������x�N�g�����擾
	direction = m_movement->CalcChaseCharacterVerocity(
		m_status,
		m_player->GetPosition(),
		m_position,
		m_moveSpeed
	);

	//��]������ݒ�
	SetRotateDirection(direction);
	//�O������ݒ�
	SetForward(direction);
}

void BossEnemyBase::AttackChaseMovement(Vector3 targetPosition, float speed)
{
	//�ړ����x���v�Z
	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		speed,
		targetPosition,
		m_position,
		m_moveSpeed
	);


	//�L�����R���ō��W���ړ�����
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//��]������ۑ�
	m_rotateDirection = m_moveSpeed;
}
