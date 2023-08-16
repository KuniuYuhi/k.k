#include "stdafx.h"
#include "Actor.h"
#include "FireBall.h"
#include "DarkWall.h"
#include "Meteo.h"
#include "AIActor.h"


Actor::Actor()
{
	//m_player = FindGO<Player>("player");
	//m_charaCon;
}

Actor::~Actor()
{
}

void Actor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void Actor::SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

void Actor::RecoveryMP()
{
	//MP�񕜏�ԂȂ�
	if (m_recoveryMpFlag == true)
	{
		if (m_status.mp < m_status.maxMp)
		{
			m_status.mp += g_gameTime->GetFrameDeltaTime();

			if (m_status.mp > m_status.maxMp)
			{
				m_status.mp = m_status.maxMp;
				//MP�񕜏�Ԃ��Ȃ��ɂ���
				m_recoveryMpFlag = false;
			}
		}
	}

	
}

Vector3 Actor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();

	//�ړ��̓���
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�_�b�V���t���O��false�Ȃ�
	if (m_dashFlag == false)
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h����Z�B
		right *= stickL.x * status.defaultSpeed;
		forward *= stickL.y * status.defaultSpeed;
	}
	else
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h�ƃ_�b�V���̒l����Z�B
		right *= stickL.x * (status.defaultSpeed + status.dashSpeed);
		forward *= stickL.y * (status.defaultSpeed + status.dashSpeed);
	}

	moveSpeed += right + forward;
	moveSpeed.y = 0.0f;
	//�l���Z�[�u���Ă���
	m_SaveMoveSpeed = moveSpeed;

	//����̃A�j���[�V�������Đ����Ȃ�ړ��Ȃ�
	if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}
	
	
}

bool Actor::CalcInvincibleTime()
{
	//���G���ԃt���O����������
	if (m_invincibleTimeFlag == true)
	{
		if (m_invincbleTime < m_invincbleTimer)
		{
			m_invincbleTimer = 0.0f;
			//�t���O��
			m_invincibleTimeFlag = false;

			//return false;
		}
		else
		{
			m_invincbleTimer += g_gameTime->GetFrameDeltaTime();
		}

		return true;
	}

	return false;
}

void Actor::DamageCollision(CharacterController& characon)
{
	//�����o������
	if (isCollisionEntable() != true)
	{
		return;
	}

	//�_�[�N�{�[���̓����蔻��
	const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkBallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			FireBall* fireball = FindGO<FireBall>("darkball");
			Damage(fireball->GetAtk());
			CreateDamageFont(fireball->GetAtk());
			return;
		}
	}

	//�_�[�N�E�H�[���̓����蔻��
	const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkWallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			DarkWall* darkwall = FindGO<DarkWall>("darkwall");
			Damage(darkwall->GetAtk());
			CreateDamageFont(darkwall->GetAtk());
			return;
		}
	}

	//���e�I�̓����蔻��
	const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MeteoCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			Meteo* meteo = FindGO<Meteo>("meteo");
			Damage(meteo->GetAtk());
			CreateDamageFont(meteo->GetAtk());
			return;
		}
	}
	//���e�I�̔����̓����蔻��
	const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MeteoExplosionCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			Meteo* meteo = FindGO<Meteo>("meteo");
			Damage(meteo->GetExplosionAttack());
			CreateDamageFont(meteo->GetExplosionAttack());
			return;
		}
	}

	//�����X�^�[�̍U���̓����蔻��
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MonsterCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			m_atttackAIActor = FindGO<AIActor>(collision->GetCreatorName());
			Damage(m_atttackAIActor->GetStatus().atk);
			CreateDamageFont(m_atttackAIActor->GetStatus().atk);
			return;
		}
	}

}

bool Actor::IsComboStateSame()
{
	//���݂̃R���{�X�e�[�g�ƃ_���[�W���󂯂����̃R���{�X�e�[�g���Ⴄ�Ȃ�
	if (GetNowComboState() != GetDamagedComboState())
	{
		//�Ⴄ
		return true;
	}
	else
		//����
		return false;
}

Quaternion Actor::Rotation()
{
	//��]�������������Ȃ�
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

bool Actor::CalcInvicibleDash()
{
	if (m_enDashInvicibleState == enDashInvicibleState_On)
	{
		if (m_invincbleDashTime < m_invincbledDashTimer)
		{
			m_invincbledDashTimer = 0.0f;
			SetInvicibleDashState(enDashInvicibleState_Off);
		}
		else
		{
			m_invincbledDashTimer += g_gameTime->GetFrameDeltaTime();
		}

		return true;
	}

	
	return false;
}

void Actor::CreateDamageFont(int damage)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Player,
		damage,
		m_position
	);
}
