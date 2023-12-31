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
		moveSpeed = diff * (status.GetDefaultSpeed() * 1.5f);
	}
	else
	{
		//���x��ݒ�
		moveSpeed = diff * status.GetDefaultSpeed();
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
	//�U���\�ȊԂ͏������Ȃ�
	if (m_attackEnableFlag == true)
	{
		return true;
	}

	if (m_attackIntervalTimer > attackintarvaltime)
	{
		//�U���\�ɂ���
		m_attackEnableFlag = true;
		//�^�C�}�[�����Z�b�g
		m_attackIntervalTimer = 0.0f;
		return true;
	}
	else
	{
		m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
	}
	return false;
	//�U��������
	//if (m_attackEnableFlag == true)
	//{
	//	//�^�C�}�[���C���^�[�o���𒴂�����
	//	if (attackintarvaltime < m_attackIntervalTimer)
	//	{
	//		//�U���\�ɂ���
	//		m_attackEnableFlag = false;
	//		//�^�C�}�[�����Z�b�g
	//		m_attackIntervalTimer = 0.0f;
	//	}
	//	else
	//	{
	//		m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
	//		//�U���s�\
	//		return false;
	//	}
	//}
	////�U���\
	//return true;
}

bool MonsterBase::AngleChangeTimeIntarval(float LimitTime)
{
	//������ς���t���O���������Ȃ�
	if (m_angleChangeTimeFlag == true)
	{
		//�^�C�}�[���������������
		if (LimitTime < m_angleChangeTimer)
		{
			//�t���O�ƃ^�C�}�[�����Z�b�g
			m_angleChangeTimeFlag = false;
			m_angleChangeTimer = 0.0f;
			return false;
		}
		else
		{
			m_angleChangeTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	return true;
}

void MonsterBase::CreateDamageFont(int damage)
{
	//�󂯂�_���[�W��\���I�u�W�F�N�g�𐶐�
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Monster,
		damage,
		m_position
	);
}

void MonsterBase::DamageCollision(CharacterController& characon)
{
	//�����蔻������Ȃ�����
	if (IsCollisionDetection() == true)
	{
		return;
	}

	//�ʏ�U���̓����蔻��
	const auto& Attack_Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	for (auto collision : Attack_Collisions)
	{
		//�L�����R���Ɠ����蔻�肪�Փ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitNormalAttack();
			return;
		}
	}

	//�X�L���̓����蔻��
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("skillAttack");
	for (auto collision : SkillCollisions)
	{
		//�L�����R���Ɠ����蔻�肪�Փ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitSkillAttack();
			return;
		}
	}
}

void MonsterBase::HitNormalAttack()
{
	m_damage = m_player->GetAtk();
	//�_���[�W���󂯂鏈��
	Damage(m_damage);
	//�_���[�W��\��
	CreateDamageFont(m_damage);
	//�G�t�F�N�g���Đ�
	CreateHitEffect();
	m_player->SetAttackHitFlag(true);
}

void MonsterBase::HitSkillAttack()
{
	m_damage = m_player->GetAtk();
	//�_���[�W���󂯂鏈��
	Damage(m_damage);
	//�_���[�W��\��
	CreateDamageFont(m_damage);
	//�G�t�F�N�g���Đ�
	CreateHitEffect();
	m_player->SetAttackHitFlag(true);
}

bool MonsterBase::IsCollisionDetection()
{
	//����̃A�j���[�V�������Đ����Ȃ�
	if (isAnimationEnable() != true)
	{
		//��������Ȃ�
		return true;
	}
	//������Ƃ�
	return false;
}

Quaternion MonsterBase::Rotation(float rotSpeed, float rotOnlySpeed)
{
	//�^�[�Q�b�g(�v���C���[)�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_targetPosition - m_position;

	m_moveSpeed = diff;

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
