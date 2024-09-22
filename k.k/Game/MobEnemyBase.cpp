#include "stdafx.h"
#include "MobEnemyBase.h"

#include "GameSceneManager.h"

////////////////////////////////////////////////////
//�R���|�[�l���g
#include "MobEnemyMovement.h"
#include "DamageProvider.h"

/////////////////////////////////////////////////////
#include "DamageFont.h"

#include "KnockBackInfoManager.h"
#include "Brave.h"

#include "UseEffect.h"


//�h���b�v�A�C�e��
#include "AttackEffect.h"
#include "RecoveryEffect.h"
#include "StaminaEffect.h"


float MobEnemyBase::GetDistanceToTargetPositionValue(Vector3 target)
{
	return CalcDistanceToTargetPosition(target);
}

bool MobEnemyBase::IsStopRequested()
{
	//���s����������
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//���S������
	if (IsDie()) return true;

	//�����܂ŗ����珈���͎~�܂�Ȃ�
	return false;
}

void MobEnemyBase::SettingDefaultComponent()
{
	//���u�G�l�~�[�ړ��R���|�[�l���g
	AddComponent<MobEnemyMovement>();
	m_movement = GetComponent<MobEnemyMovement>();
	//�_���[�W�v���o�C�_�[�R���|�[�l���g
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
	//���g�̃C���X�^���X��ۑ�
	m_damageProvider->SetProviderCharacterInstance(this);
}


void MobEnemyBase::DieFromDamage()
{
	//���g��Ԃ�
	ReleaseThis();

	//���S�����Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Mob_Die, g_soundManager->GetSEVolume());

	//���S�G�t�F�N�g����
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 3.0f, Quaternion::Identity, false);

}

bool MobEnemyBase::IsDropBuffItem()
{
	int ram = rand() % 10;

	if (ram > 3)
	{
		return true;
	}

	return false;
}

void MobEnemyBase::DropBuffItem()
{
	int ram = rand() % 2;

	if (ram > 0)
	{
		//�U���A�C�e���𗎂Ƃ�
		AttackEffect* at = NewGO<AttackEffect>(0, "AttackEffect");
		at->SetStartPosition(m_position);
		return;
	}
	
	ram = rand() % 2;

	//�X�^�~�i�̓v���C���[�̃X�^�~�i�������Ă��鎞�������Ƃ�
	if (ram > 0 && m_player->GetStatus().GetCurrentStamina() <= 100)
	{
		//�X�^�~�i�񕜃A�C�e���𗎂Ƃ�
		StaminaEffect* se = NewGO<StaminaEffect>(0, "StaminaEffect");
		se->SetStartPosition(m_position);
	}
	else
	{
		//�񕜃A�C�e���𗎂Ƃ�
		RecoveryEffect* re = NewGO<RecoveryEffect>(0, "RecoveryEffect");
		re->SetStartPosition(m_position);

	}

}

void MobEnemyBase::CheckSelfCollision()
{
	//��_���[�W���󂯂Ȃ�����
	//if (adadadadada) return;



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
			CreateDamageFont(dp->GetAdjustedDamage(),DamageFont::enDamageActor_Monster);
		
			//��_���[�W�������s�������ɏ�������
			ProcessHit(dp->GetProviderDamageInfo());

			//�����ID��O�t���[����ID�ɕۑ�
			oldAttackId = currentAttackId;
		}
	}


}

void MobEnemyBase::TurnToTarget()
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

	direction = m_player->GetPosition() - m_position;

	//��]������ݒ�
	SetRotateDirection(direction);
	//�O������ݒ�
	SetForward(direction);
}

void MobEnemyBase::ChaseMovement(Vector3 targetPosition, bool isBossPosCheck)
{
	//�A�N�V�������͈ړ��������Ȃ�
	if (IsAction()) return;

	//����������悤�R���|�[�l���g�������Ă���Ȃ炻�����̈ړ�������D��

	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed,
		isBossPosCheck
	);

	//�ړ��ʂ�����ΑO������ݒ�
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetForward(m_moveSpeed);
	}
	
	//�v���C���[�Ɍ������������v�Z
	float toPlayerDistance = CalcDistanceToTargetPosition(targetPosition);
	bool isExecute = true;

	Vector3 tempMoveSpeed = m_moveSpeed;
	//�ҋ@�t���O������Ȃ�
	if (m_isWaitingFlag)
	{
		//�v���C���[�Ƃ̋������ҋ@���鋗�����傫���Ȃ�L�����R���̏��������Ȃ��悤�ɂ���
		if (toPlayerDistance < m_status.GetWaitingDistance())
		{
			isExecute = false;
			m_moveSpeed = g_vec3Zero;
		}
	}
	else
	{
		//�v���C���[�Ƃ̋������ڋ߂��鋗�����傫���Ȃ�L�����R���̏��������Ȃ��悤�ɂ���
		if (toPlayerDistance < m_status.GetApproachDistance())
		{
			isExecute = false;
			m_moveSpeed = g_vec3Zero;
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
	
	

	//��]������ۑ�
	m_rotateDirection = tempMoveSpeed;
}

void MobEnemyBase::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}

	//���`��Ԃŉ�]����������v�Z�B���X�ɉ�]������
	/*m_currentRotDirection = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * 3.0f, m_currentRotDirection, m_rotateDirection
	);*/

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

	//�O��������]�����ɍ��킹��
	//m_forward = m_currentRotDirection;
}

void MobEnemyBase::SettingKnockBackProcess()
{
	m_curvePointList.clear();
	//�m�b�N�o�b�N�J�[�u�̃|�C���g���X�g���擾
	m_curvePointList = KnockBackInfoManager::GetInstance()->GetCurvePoint(
		m_hitKnockBackPattern
	);

	//����̃p�^�[���̃m�b�N�o�b�N�̃X�s�[�h���擾
	m_knockBackSpeed = KnockBackInfoManager::
		GetInstance()->GetStatus().GetKnockBackSpeed(m_hitKnockBackPattern);

	//�O�����𐳋K�����ĕ����𔽓]������
	Vector3 diff = m_position - m_player->GetPosition();
	m_forward = diff;
	m_forward.Normalize();

	//�d���^�C�}�[���Z�b�g
	m_starkTimer = 0.0f;
}

void MobEnemyBase::KnockBackMove(int listNum)
{
	//���K��
	m_curvePointList[listNum].curvePosition.Normalize();
	//�ړ����x���v�Z
	m_moveSpeed.x = m_forward.x * m_curvePointList[listNum].curvePosition.x * m_knockBackSpeed.x;
	m_moveSpeed.y = m_curvePointList[listNum].curvePosition.y * m_knockBackSpeed.y;
	m_moveSpeed.z = m_forward.z * m_curvePointList[listNum].curvePosition.z * m_knockBackSpeed.x;

	//�L�����R�����g���Ĉړ�
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void MobEnemyBase::KnockBackGravityFall()
{
	//�d�͂̌v�Z
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//�L�����R�����g���č��W���ړ�
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.get()->IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
}

bool MobEnemyBase::IsAttackable()
{
	//�^�C�}�[���U���C���^�[�o���𒴂�����
	if (m_attackIntarvalTimer >= m_status.GetAttackIntarval())
	{
		//�U���\
		return true;
	}
	//�^�C�}�[�����Z
	m_attackIntarvalTimer += g_gameTime->GetFrameDeltaTime();

	return false;
}

void MobEnemyBase::PlayHitSound()
{
	if (IsDie())
	{
		//���ꂽ�ꍇ�͋��߂̉����o��
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Monster_DieHit,
			g_soundManager->GetSEVolume()
		);

		return;
	}

	//���Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Monster_Hit,
		g_soundManager->GetSEVolume()
	);

}

