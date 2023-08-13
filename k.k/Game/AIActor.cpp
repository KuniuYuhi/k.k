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

Vector3 AIActor::calcVelocity(Status status,Vector3 targetposition)
{
	Vector3 moveSpeed = Vector3::Zero;

	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = targetposition - m_position;
	
	//���K��
	diff.Normalize();
	
	diff.y = 0.0f;
	//���x��ݒ�
	moveSpeed = diff * status.defaultSpeed;
	//�O����
	m_forward = diff;
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

void AIActor::CreateDamageFont(int damage)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Monster, 
		damage,
		m_position
	);
}

void AIActor::DamageCollision(CharacterController& characon)
{
	//�����o������
	if (isAnimationEntable() != true)
	{
		return;
	}

	//�ʏ�U���̓����蔻��
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : Attack_1Collisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitNormalAttack();
			return;
		}
	}

	//�q�[���[�̃X�L���̓����蔻��
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : SkillCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitHeroSkillAttack();
			return;
		}
	}

	//�E�B�U�[�h�̃t�@�C���[�{�[���̓����蔻��
	const auto& FireBallCollisions = g_collisionObjectManager->FindCollisionObjects("fireball");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : FireBallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitFireBall();
			return;
		}
	}

	//�E�B�U�[�h�̃t���C���s���[�̓����蔻��
	const auto& FlamePillarCollisions = g_collisionObjectManager->FindCollisionObjects("flamepillar");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : FlamePillarCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			HitFlamePillar();
			return;
		}
	}
}

void AIActor::HitNormalAttack()
{
	//�_���[�W���󂯂�
	Damage(m_player->GetAtk());
	CreateDamageFont(m_player->GetAtk());
}

void AIActor::HitHeroSkillAttack()
{
	//�_���[�W���󂯂�
	Damage(m_player->GetSkillAtk());
	CreateDamageFont(m_player->GetSkillAtk());
}

void AIActor::HitFireBall()
{
}

void AIActor::HitFlamePillar()
{
}

bool AIActor::AttackInterval(const float attackintarvaltime)
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

bool AIActor::DamageInterval(const float damageintarvaltime)
{
	//�_���[�W���󂯂���
	if (m_damageFlag == true)
	{
		//�^�C�}�[���C���^�[�o���𒴂�����
		if (damageintarvaltime < m_damageIntervalTimer)
		{
			//�_���[�W���󂯂���悤�ɂ���
			m_damageFlag = false;
			//�^�C�}�[�����Z�b�g
			m_damageIntervalTimer = 0.0f;
		}
		else
		{
			m_damageIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//�_���[�W���󂯂Ȃ�
			return false;
		}
	}
	//�_���[�W���󂯂���悤�ɂ���
	return true;
}

bool AIActor::AngleChangeTimeIntarval(float LimitTime)
{
	//�x�N�g�����v�Z������
	if (m_angleChangeTimeFlag == true)
	{
		//�^�C�}�[���C���^�[�o���𒴂�����
		if (LimitTime < m_angleChangeTimer)
		{
			//�x�N�g�����v�Z�ł���悤�ɂ���
			m_angleChangeTimeFlag = false;
			//�^�C�}�[�����Z�b�g
			m_angleChangeTimer = 0.0f;
			//�x�N�g�����v�Z�s�\
			return false;
		}
		else
		{
			m_angleChangeTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	//�x�N�g�����v�Z�\
	return true;
}

bool AIActor::IsFindPlayer(float distance)
{
	//�^�[�Q�b�g(�v���C���[)�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	//���g����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����
	Vector3 diff = m_targetPosition - m_position;
	
	diff.y = 0.0f;
	//�x�N�g�����Z�[�u
	m_toTarget = diff;
	//�O������ۑ�
	/*m_forward = diff;
	m_forward.Normalize();*/

	//�v�Z�����x�N�g��������̋�����菬����������
	if (diff.Length() < distance)
	{
		//������
		return true;
	}
	else
	{
		//diff.Normalize();
		////�O������ۑ�
		//m_forward = diff;
		//������Ȃ�����
		m_targetPosition = Vector3::Zero;
		return false;
	}


	return false;
}

Quaternion AIActor::Rotation()
{
	//todo target�ɂ�����^�[�Q�b�g�Ɍ������܂܉�]

	if (RotationOnly() == true)
	{
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		return m_rotation;
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return m_rotation;
}
