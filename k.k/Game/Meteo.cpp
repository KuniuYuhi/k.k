#include "stdafx.h"
#include "Meteo.h"
#include "InitEffect.h"

#include "Player.h"

namespace {
	const float EXPLOSION_SCALE = 12.0f;
	const Vector3 SCALE = { 16.0f,16.0f,16.0f };

	const float DOWN = -20.0f;
	const float DELETE_DOWN = -70.0f;

	const float METEO_COLLISION_SIZE = 30.0f;
	const float METEO_EFFECT_SIZE = 8.0f;
	const float RANGE_EFFECT_SIZE = 35.0f;
	//0.0065f
	const float ADD_TIMER = 0.0065f;

	const float MUL_EXPLOSION_SCALE = 2.4f;

	const float EXPLOSION_COLLISION_SIZE = 330.0f;

	const float EXPLOSION_Y_POS = 20.0f;


	const float GROUND_CHECK_COLLISION_SIZE = 4.0f;

	const float CENTER_POS_ADD_Y = 200.0f;

	const float GRAVITY = 20.0f;


	const float DIV_DISTANCE = 200.0f;


	const int ATTACK = 60;
	const int LOWEAT_ATTACK = 20;
}

Meteo::Meteo()
{
}

Meteo::~Meteo()
{
	if (m_meteoEffect != nullptr)
	{
		m_meteoEffect->Stop();
	}
	if (m_ExplosionEffect != nullptr)
	{
		m_ExplosionEffect->Stop();
	}
	if (m_rangeEffect != nullptr)
	{
		m_rangeEffect->Stop();
	}

	//DeleteGO(m_collision);
	DeleteGO(m_explosionCollision);
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			isHit = false;
			return 0.0f;
		}
		else
		{
			//�n�ʂƂԂ�������B
		//�t���O��true�ɁB
			isHit = true;
			return 0.0f;
		}
	}
};

bool Meteo::Start()
{
	//�U���͂̐ݒ�
	SetAttack(ATTACK);

	m_scale = SCALE;

	SettingMeteoRoute();

	//���e�I�G�t�F�N�g�Đ�
	PlayMeteoEffect();

	PlayRangeEffect();

    return true;
}

void Meteo::Update()
{
	ManageState();
}

int Meteo::CalcDamageToDistance(const Vector3 targetPosition)
{
	//�x�N�g�������߂�
	Vector3 diff = targetPosition - m_position;
	diff.y = 0.0f;
	float length = diff.Length();

	//������200���ƂɃ_���[�W�����炷
	float reduce = length / DIV_DISTANCE;

	int attack = m_attack; 
	attack -= reduce * 10.0f;

	//�Œ�ł�20�_���[�W�ɂȂ�悤�ɂ���
	if (attack < LOWEAT_ATTACK)
	{
		attack = LOWEAT_ATTACK;
	}

	return attack;
}

void Meteo::Move()
{
	//if (m_deleteTimer < m_flightDuration)
	//{
	//	float X = m_forward.x * m_meteoVerocity.x *
	//		g_gameTime->GetFrameDeltaTime() * 1.0f;
	//	float Z = m_forward.z * m_meteoVerocity.x *
	//		g_gameTime->GetFrameDeltaTime() * 1.0f;

	//	m_movePos += {
	//		X,
	//			(m_meteoVerocity.y - (GRAVITY * m_deleteTimer))*
	//			g_gameTime->GetFrameDeltaTime() * 5.0f,
	//			Z
	//	};

	//	m_deleteTimer += g_gameTime->GetFrameDeltaTime() * 1.0f;
	//}
	//else
	//{
	//	//�n�ʂɂ���
	//	//�����p�����蔻��̐���
	//	CreateExplosionCollision();
	//	//��������
	//	SetExplosionFlag(true);

	//	//�����G�t�F�N�g�̐���
	//	while (m_ExplosionEffect == nullptr)
	//	{
	//		//��������
	//		m_explosionEffectFlag = true;
	//		Explosion();
	//	}

	//	//���̃X�e�[�g�ɑJ��
	//	m_state = enExplosionState;
	//	return;
	//}
	

	//���`��Ԃ��g���Ĉړ�����
	//���_���璆�_
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//���_����I�_
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//���`��Ԃ����Q�̃x�N�g�����X�ɐ��`���
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);
	
	m_timer += ADD_TIMER;

	//�ݒ�ƍX�V
	m_meteoEffect->SetPosition(m_movePos);
	m_meteoEffect->Update();
	
	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//�����͈͂̓����蔻��쐬
	auto explosionCollision = NewGO<CollisionObject>(0, GetCollisionName());
	explosionCollision->CreateSphere(
		m_position,
		g_quatIdentity,
		EXPLOSION_COLLISION_SIZE
	);
	explosionCollision->SetPosition(m_position);
	//�����蔻��𐶐������쐬�҂̐ݒ�
	explosionCollision->SetCreatorName(GetName());
	//�����ō폜����悤�ɂ���
	explosionCollision->SetIsEnableAutoDelete(true);

	explosionCollision->Update();
}

bool Meteo::IsGroundCheck(const float downValue)
{
	BoxCollider m_boxCollider;
	m_boxCollider.Create(g_vec3One * GROUND_CHECK_COLLISION_SIZE);
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(m_position.x, m_position.y + downValue, m_position.z));
	//�ǂ̔����Ԃ�
	IsGroundResult callback_Ground;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
		start, end, callback_Ground);
	//�n�ʂɏՓ˂���
	if (callback_Ground.isHit == true)
	{
		return true;
	}
	else
	{
		//�n�ʂł͂Ȃ�����
		return false;
	}
}

void Meteo::Explosion()
{
	//�G�t�F�N�g����
	m_ExplosionEffect = NewGO<EffectEmitter>(0);
	m_ExplosionEffect->Init(InitEffect::enEffect_Meteo_Explosion);
	m_ExplosionEffect->Play();
	m_ExplosionEffect->SetPosition(m_position);
	m_ExplosionEffect->Update();
}

const int Meteo::GetAttack()
{
	//�v���C���[�̍��W���擾
	Vector3 playerPos = FindGO<Player>("player")->GetPosition();
	//�v���C���[�̍��W�ɂ���ă_���[�W�ύX
	return CalcDamageToDistance(playerPos);
}

void Meteo::PlayMeteoEffect()
{
	m_meteoEffect = NewGO<EffectEmitter>(0);
	m_meteoEffect->Init(InitEffect::enEffect_Meteo);
	m_meteoEffect->Play();
	m_meteoEffect->SetScale(g_vec3One * METEO_EFFECT_SIZE);
	m_meteoEffect->SetPosition(m_position);
	m_meteoEffect->Update();
}

void Meteo::SettingMeteoRoute()
{
	m_movePos = m_position;
	//�n�_�����߂�
	m_startPosition = m_position;
	//���ԓ_�����߂�
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;

	//���ԓ_��Y���W���グ��
	m_centerPosition.y = m_startPosition.y + CENTER_POS_ADD_Y;

	//1.�ڕW�Ɍ����������̌v�Z
	//�I�_-�n�_
	//Vector3 targetDistance = m_targetPosition - m_startPosition;
	////�O������ݒ�
	//m_forward = targetDistance;
	//m_forward.Normalize();
	//float distance = targetDistance.Length();
	////���e�I������������p�x�����߂�
	//Vector3 toAngle = targetDistance;
	//toAngle.Normalize();
	//toAngle.Length();
	//m_angle = Math::Lerp(toAngle.Length(), 60.0f, 30.0f);

	////2.�����x�̌v�Z
	//float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	////3.�����x�̕���
	//m_meteoVerocity.x = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	//m_meteoVerocity.y = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	////4.��s���Ԃ̌v�Z
	//m_flightDuration = distance / m_meteoVerocity.x;

}

void Meteo::PlayRangeEffect()
{
	Vector3 pos = m_targetPosition;
	pos.y += 3.0f;

	m_rangeEffect = NewGO<EffectEmitter>(0);
	m_rangeEffect->Init(InitEffect::enEffect_Meteo_Range);
	m_rangeEffect->Play();
	m_rangeEffect->SetScale(g_vec3One * RANGE_EFFECT_SIZE);
	m_rangeEffect->SetPosition(pos);
	m_rangeEffect->Update();
}

void Meteo::ManageState()
{
	switch (m_state)
	{
	case Meteo::enMoveState:
		OnProcessMoveTransition();
		break;
	case Meteo::enExplosionState:
		OnProcessExplosionTransition();
		break;
	default:
		break;
	}
}

void Meteo::OnProcessMoveTransition()
{
	//�n�ʂɒ�������
	if (IsGroundCheck(DOWN) == true || m_movePos.y <= EXPLOSION_Y_POS)
	{
		//�n�ʂɂ���
		//�����p�����蔻��̐���
		CreateExplosionCollision();
		//��������
		SetExplosionFlag(true);

		//�����G�t�F�N�g�̐���
		while (m_ExplosionEffect == nullptr)
		{
			//��������
			m_explosionEffectFlag = true;
			Explosion();
		}

		//���̃X�e�[�g�ɑJ��
		m_state = enExplosionState;
		return;
	}
	//�ړ�����
	Move();
}

void Meteo::OnProcessExplosionTransition()
{
	//�������Ă���̏���
	if (GetExplosionFlag() == true)
	{
		CalcDeleteTime();
	}

}

void Meteo::CalcDeleteTime()
{
	if (m_explosionEndTime < m_explosionEndTimer)
	{
		DeleteGO(this);
	}
	else
	{
		m_explosionEndTimer += g_gameTime->GetFrameDeltaTime();

		m_ExplosionEffect->SetScale(m_scale * (m_explosionEndTimer * MUL_EXPLOSION_SCALE));
		m_ExplosionEffect->Update();
	}	
}
