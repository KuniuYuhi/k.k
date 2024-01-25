#include "stdafx.h"
#include "Actor.h"
#include "DarkWall.h"
#include "DarkBall.h"
#include "Meteo.h"
#include "DarkMeteorite.h"
#include "DarkSpearObj.h"

#include "AIActor.h"
#include "MagicBall.h"

#include "IAttackObject.h"


Actor::Actor()
{
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

bool Actor::IsFlashing()
{
	//���G���Ԃ�
	if (m_invincibleTimeFlag == true)
	{
		//���f����draw�t���O��true�Ȃ�\��(false)�B
		if (m_modelDrawFlag == true)
		{
			m_modelDrawFlag = false;
			return false;
		}
		else
		{
			//���f����draw�t���O��false�Ȃ̂Ŕ�\��(true)�B
			m_modelDrawFlag = !m_modelDrawFlag;
			return true;
		}
	}
	else
	{
		//���G���ԂłȂ��Ȃ�\��
		return false;
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
	right.Normalize();
	//�ړ��̓���
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�_�b�V���t���O��false�Ȃ�
	if (m_dashFlag == false)
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h����Z�B
		right *= stickL.x * status.GetDefaultSpeed();
		forward *= stickL.y * status.GetDefaultSpeed();
	}
	else
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h�ƃ_�b�V���̒l����Z�B
		right *= stickL.x * (status.GetDefaultSpeed() + status.GetDashSpeed());
		forward *= stickL.y * (status.GetDefaultSpeed() + status.GetDashSpeed());
	}

	moveSpeed += right + forward;
	
	//�l���Z�[�u���Ă���
	m_SaveMoveSpeed = moveSpeed;

	return moveSpeed;
}

void Actor::CalcForward(Vector3 moveSpeed)
{
	//�O�����̎擾
	//x,y�ǂ��炩�̓��͂���������
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		m_forward = moveSpeed;
		m_forward.Normalize();

		//�O������ݒ�ł�������Ȃ�
		if (IsSetForwardCondition() == true)
		{
			
		}
	}
}

bool Actor::CalcInvincibleTime()
{
	//���G���ԃt���O���Z�b�g����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_invincibleTimeFlag != true)
	{
		return false;
	}

	//���G���ԃt���O����������
	if (m_invincbleTime < m_invincbleTimer)
	{
		m_invincbleTimer = 0.0f;
		//�t���O��������B
		m_invincibleTimeFlag = false;
		return false;
	}
	else
	{
		m_invincbleTimer += g_gameTime->GetFrameDeltaTime();
		//���G���ԂȂ̂�true��������
		return true;

	}
}

void Actor::DamageCollision(CharacterController& characon)
{
	//�����蔻����Ƃ邩
	if (IsDecisionCollision() == false)
	{
		return;
	}

	//�A�^�b�N�I�u�W�F�N�g�̓����蔻��
	const auto& DarkBallCollisions = 
		g_collisionObjectManager->FindCollisionObjects("attackobject");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkBallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			//���������I�u�W�F�N�g�̐���҂̃C���X�^���X���擾
			auto* attackObject = 
				FindGO<IAttackObject>(collision->GetCreatorName());
			int attack = attackObject->GetAttack();
			//��_���[�W����
			Damage(attack);
			//�t�H���g����
			CreateDamageFont(m_hitDamage);

			return;
		}
	}




	///////////////////////////////////////////////

	//�_�[�N�{�[���̓����蔻��
	//const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : DarkBallCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		//todo ��ɐ^�񒆂Ŕ�����Ƃ��Ă��܂�
	//		DarkBall* darkball = FindGO<DarkBall>("darkball");
	//		//�Ԃ������̂Ńt���O�𗧂Ă�
	//		darkball->SetHitFlag(true);
	//		Damage(darkball->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////�_�[�N�E�H�[���̓����蔻��
	//const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : DarkWallCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkWall* darkwall = FindGO<DarkWall>("darkwall");
	//		Damage(darkwall->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////�_�[�N�X�s�A�̓����蔻��
	//const auto& DarkSpearCollisions = g_collisionObjectManager->FindCollisionObjects("darkspear");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : DarkSpearCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkSpearObj* darkSpearObj = FindGO<DarkSpearObj>("darkspearobj");
	//		Damage(darkSpearObj->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////���e�I�̓����蔻��
	//const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : MeteoCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		Meteo* meteo = FindGO<Meteo>("meteo");
	//		Damage(meteo->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		//���e�I�ɓ��������̂ŋ����I�ɔ���������
	//		meteo->Explosion();
	//		return;
	//	}
	//}
	////���e�I�̔����̓����蔻��
	//const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : MeteoExplosionCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		Meteo* meteo = FindGO<Meteo>("meteo");
	//		int damage = meteo->CalcDamageToDistance(m_position);
	//		Damage(damage);
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	//�_�[�N���e�I�̍U���̓����蔻��
	//const auto& DarkMeteoCollisions = g_collisionObjectManager->FindCollisionObjects("bigmeteo");
	////�R���W�����̔z���for���ŉ�
	//for (auto collision : DarkMeteoCollisions)
	//{
	//	//���g�̃L�����R���ƏՓ˂�����
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkMeteorite* darkMeteo = FindGO<DarkMeteorite>("darkmeteorite");
	//		Damage(darkMeteo->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		darkMeteo->SetChaseFlag(true);
	//		return;
	//	}
	//}

	//�����X�^�[�̍U���̓����蔻��
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MonsterCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			m_atttackAIActor = FindGO<AIActor>(collision->GetCreatorName());
			//�q�b�g�����Đ�
			m_atttackAIActor->PlayAttackSound();
			Damage(m_atttackAIActor->GetStatus().GetAtk());
			CreateDamageFont(m_hitDamage);
			return;
		}
	}

	

}

bool Actor::IsComboStateSame()
{
	//�v���C���[�̃R���{��Ԃ�
	// �G���U�����󂯂����̃R���{��Ԃ��Ⴄ�Ȃ�
	if (GetNowComboState() != GetDamagedComboState())
	{
		return true;
	}
	return false;
}

Quaternion Actor::Rotation(float rotSpeed,float rotOnlySpeed)
{
	//��]�������������Ȃ�
	if (RotationOnly() == true)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_forward.x) >= 0.001f || fabsf(m_forward.z) >= 0.001f)
		{
			//m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
			m_rotation.SetRotationYFromDirectionXZ(m_forward);

			/*Vector3 forward;
			m_rotation.Apply(forward);
			Vector3 diff = forward - m_position;
			m_forward = diff;
			m_forward.y = 0.0f;
			m_forward.Normalize();*/
		}
		return m_rotation;
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_forward.x) >= 0.001f || fabsf(m_forward.z) >= 0.001f)
	{
		//m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_SaveMoveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_forward);

		/*Vector3 forward;
		m_rotation.Apply(forward);
		Vector3 diff = forward - m_position;
		m_forward = diff;
		m_forward.y = 0.0f;
		m_forward.Normalize();*/
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
	//�_���[�W��0��0�ȉ��Ȃ�t�H���g���o���Ȃ�
	if (damage <= 0)
	{
		return;
	}

	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Player,
		damage,
		m_position
	);
}

bool Actor::IsDecisionCollision()
{
	//���G��ԂȂ珈�����Ȃ�
	if (GetInvicibleFlag() == true)
	{
		return false;
	}

	//���G���ԂȂ珈�����Ȃ�
	if (GetInvincibleTimeFlag() == true)
	{
		return false;
	}
	//�����蔻�肵�Ȃ��A�j���[�V�����Ȃ珈�����Ȃ�
	if (isCollisionEntable() == true)
	{
		return false;
	}
	//������Ƃ�
	return true;
}
