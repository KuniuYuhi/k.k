#include "stdafx.h"
#include "Meteo.h"
#include "InitEffect.h"

//todo ���e�I����������Ƃ��������~�܂�

namespace {
	const float EXPLOSION_SCALE = 12.0f;
	const Vector3 SCALE = { 16.0f,16.0f,16.0f };

	const float DOWN = -20.0f;
	const float DELETE_DOWN = -70.0f;
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

	DeleteGO(m_collision);
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
	//m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm", nullptr, 0, enModelUpAxisZ, false, false, false);

	
	m_movePos = m_position;
	//�n�_�����߂�
	m_startPosition = m_position;
	//���ԓ_�����߂�
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;
	//Y���W���グ��
	m_centerPosition.y += m_yUp;

	m_scale = SCALE;

	//m_model.SetTransform(m_position, m_rotation, m_scale);
	//m_model.Update();


	m_meteoEffect = NewGO<EffectEmitter>(0);
	m_meteoEffect->Init(InitEffect::enEffect_Meteo);
	m_meteoEffect->Play();
	m_meteoEffect->SetScale({ 8.0f,8.0f,8.0f });
	m_meteoEffect->SetPosition(m_position);
	m_meteoEffect->Update();


	//���e�I�̓����蔻��̐���
	m_collision = NewGO<CollisionObject>(0, "meteo");
	m_collision->CreateSphere(
		m_position,
		m_rotation,
		30.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();

    return true;
}

void Meteo::Update()
{
	//�������Ă���̏���
	if (GetExplosionFlag() == true)
	{
		CalcDeleteTime();
		return;
	}


	if (m_explosionEffectFlag!=true && m_collision->IsDead() != true)
	{
		if (IsGroundCheck(DELETE_DOWN))
		{
			m_collision->Dead();
			//��������
			m_explosionEffectFlag = true;
			//�����G�t�F�N�g�̐���
			while (m_ExplosionEffect == nullptr)
			{
				Explosion();
			}
			
		}
	}
	
	//��������܂ł̏���
	//�n�ʂɂ��܂�
	if(IsGroundCheck(DOWN)==true || m_movePos.y <= 0.0f)
	{
		//�����p�����蔻��̐���
		CreateExplosionCollision();
		//��������
		SetExplosionFlag(true);
	}
	else
	{
		Move();
	}
}

int Meteo::CalcDamageToDistance(const Vector3 targetPosition)
{
	//�x�N�g�������߂�
	Vector3 diff = targetPosition - m_position;
	diff.y = 0.0f;
	float length = diff.Length();

	//100���ƂɃ_���[�W�����炷
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
	//
	m_timer += 0.01f*0.65f;

	//�ݒ�ƍX�V
	//�������Ă��Ȃ��ԃ��e�I�̓����蔻��̍��W���X�V����
	if (m_explosionEffectFlag != true)
	{
		m_collision->SetPosition(m_movePos);
		m_collision->Update();
	}
	

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
		330.0f
	);
	explosionCollision->SetPosition(m_position);

	

}

bool Meteo::IsGroundCheck(const float downValue)
{
	BoxCollider m_boxCollider;
	m_boxCollider.Create(Vector3(5.0f, 5.0f, 5.0f));
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

void Meteo::Render(RenderContext& rc)
{
	//m_model.Draw(rc);
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

		m_ExplosionEffect->SetScale(m_scale * (m_explosionEndTimer*2.4f));
		m_ExplosionEffect->Update();
	}	
}
