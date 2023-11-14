#include "stdafx.h"
#include "Meteo.h"
#include "InitEffect.h"

namespace {
	const float EXPLOSION_SCALE = 12.0f;
	const Vector3 SCALE = { 16.0f,16.0f,16.0f };

	const float DOWN = -20.0f;
	const float DELETE_DOWN = -70.0f;

	const float METEO_COLLISION_SIZE = 30.0f;
	const float METEO_EFFECT_SIZE = 8.0f;
	//0.0065f
	const float ADD_TIMER = 0.0065f;

	const float MUL_EXPLOSION_SCALE = 2.4f;

	const float EXPLOSION_COLLISION_SIZE = 330.0f;

	const float EXPLOSION_Y_POS = 20.0f;


	const float GROUND_CHECK_COLLISION_SIZE = 4.0f;

	const float CENTER_POS_ADD_Y = 200.0f;
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
	m_movePos = m_position;
	//�n�_�����߂�
	m_startPosition = m_position;
	//���ԓ_�����߂�
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;

	//���ԓ_��Y���W���グ��
	m_centerPosition.y = m_startPosition.y + CENTER_POS_ADD_Y;

	m_scale = SCALE;

	m_meteoEffect = NewGO<EffectEmitter>(0);
	m_meteoEffect->Init(InitEffect::enEffect_Meteo);
	m_meteoEffect->Play();
	m_meteoEffect->SetScale(g_vec3One * METEO_EFFECT_SIZE);
	m_meteoEffect->SetPosition(m_position);
	m_meteoEffect->Update();


	//���e�I�̓����蔻��̐���
	/*m_collision = NewGO<CollisionObject>(0, "meteo");
	m_collision->CreateSphere(
		m_position,
		m_rotation,
		METEO_COLLISION_SIZE
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();*/

	//�X�e�[�g�ݒ�
	m_state = enMoveState;

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

	//200���ƂɃ_���[�W�����炷
	float reduce = length / 200.0f;

	int attack = m_attack; 
	attack -= reduce * 10.0f;

	if (attack <= 0)
	{
		attack = 20;
	}

	return attack;
}

void Meteo::Move()
{
	//���`��Ԃ��g���Ĉړ�����
	//���_���璆�_
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//���_����I�_
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//���`��Ԃ����Q�̃x�N�g�����X�ɐ��`���
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);
	
	m_timer += ADD_TIMER;

	//�ݒ�ƍX�V
	//�������Ă��Ȃ��ԃ��e�I�̓����蔻��̍��W���X�V����
	/*m_collision->SetPosition(m_movePos);
	m_collision->Update();*/

	m_meteoEffect->SetPosition(m_movePos);
	m_meteoEffect->Update();
	
	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//�����͈͂̓����蔻��쐬
	auto explosionCollision = NewGO<CollisionObject>(0, "explosion");
	explosionCollision->CreateSphere(
		m_position,
		m_rotation,
		EXPLOSION_COLLISION_SIZE
	);
	explosionCollision->SetPosition(m_position);
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

		//���e�I�{�̂̓����蔻��폜
		//DeleteGO(m_collision);

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
