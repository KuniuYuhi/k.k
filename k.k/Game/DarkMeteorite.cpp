#include "stdafx.h"
#include "DarkMeteorite.h"
#include "MagicBallMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"

namespace {
	const int COLLISION_RADIUS = 550.0f;

	const float END_Y_ADD_VALUE = 320.0f;


	const float METEO_EFFECT_SIZE = 80.0f;
	const float METEO_EXPLOSION_EFFECT_SIZE = 30.0f;

	const float ADD_Y_DOWN = 300.0f;

}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
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

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
	if (m_collision != nullptr)
	{
		DeleteGO(m_collision);
	}


	m_areaEffect = nullptr;
	m_mainEffect = nullptr;
}

bool DarkMeteorite::Start()
{
	InitModel();

	//�X�e�[�^�X�ݒ�
	m_status.InitMagicBallStatus("DarkMeteorite");

	//�R���|�[�l���g�̐ݒ�
	DefaultSettingComponents();
	AddSettingComponents();

	//���Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_Meteo_Fly,
		g_soundManager->GetSEVolume()
	);

	return true;
}


void DarkMeteorite::InitModel()
{
	
}

void DarkMeteorite::ShotStartDarkMeteorite()
{
	//�_���[�W����ݒ�
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_status.GetPower(),
		m_status.GetKnockBackTimeScale(),
		m_status.GetKnockBackPattern()
	);

	//�����蔻��쐬
	CreateCollision();

	//�G�t�F�N�g�𐶐�
	m_mainEffect = NewGO<UseEffect>(0, "DarkMeteoriteEffect");
	m_mainEffect->PlayEffect(
		enEffect_Meteo,
		m_position,
		g_vec3One * METEO_EFFECT_SIZE,
		Quaternion::Identity,
		false
	);


	m_isMove = true;
}

void DarkMeteorite::PlayRangeEffect(Vector3 position)
{
	//�G�t�F�N�g����
	m_areaEffect = NewGO<UseEffect>(0, "DarkMeteoriteAreaEffect");
	m_areaEffect->PlayEffect(
		enEffect_Meteo_Range,
		position,
		{ 90.0f,1.0f,90.0f },
		Quaternion::Identity,
		false);
}

void DarkMeteorite::AddSettingComponents()
{
	//���@���ړ��R���|�[�l���g��ǉ�
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//�ړ��R���|�[�l���g�Ɏ��g�̃C���X�^���X��ݒ�
	m_magicBallMovement->SetMagicBallInstance(this);
}

void DarkMeteorite::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(
		0, g_collisionObjectManager->m_enemyAttackCollisionName
	);
	//����̓����蔻��
	m_collision->CreateSphere(m_position, m_rotation, COLLISION_RADIUS);

	m_collision->SetCreatorName(GetName());
	//�����ō폜���Ȃ�
	m_collision->SetIsEnableAutoDelete(false);
}

void DarkMeteorite::Move()
{
	if (m_magicBallMovement == nullptr) return;

	//��������
	m_magicBallMovement->MoveFall();

	Vector3 effectPosition = m_position;

	effectPosition.y -= ADD_Y_DOWN;

	//�G�t�F�N�g���W��ݒ�
	m_mainEffect->SetMovePosition(effectPosition);

	//�����蔻��̍��W�X�V
	m_collision->SetPosition(m_position);
	m_collision->Update();
}

bool DarkMeteorite::IsDelete()
{
	if (IsGroundHit()) return true;

	return false;
}

bool DarkMeteorite::IsGroundHit()
{
	CCapsuleCollider m_capsuleCollider;
	m_capsuleCollider.Init(6.0f, 8.0f);
	//�n�_�ƏI�_��ݒ�
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(
		btVector3(m_position.x, m_position.y, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(
		btVector3(m_position.x, m_position.y - END_Y_ADD_VALUE, m_position.z));
	//�ǂ̔����Ԃ�
	IsWallResult isWallResult;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
		start, end, isWallResult);
	//�ǂɃq�b�g������
	if (isWallResult.isHit == true)
	{
		//�q�b�g����
		return true;
	}
	else
	{
		//�q�b�g���Ȃ�����
		return false;
	}


	return false;
}

void DarkMeteorite::Explosion()
{
	m_areaEffect->Delete();
	m_areaEffect = nullptr;

	m_mainEffect->Delete();
	m_mainEffect = nullptr;

	Vector3 position = m_position;
	position.y = 0.0f;

	m_mainEffect = NewGO<UseEffect>(0, "DarkMeteoriteExplosionEffect");
	//�����G�t�F�N�g
	m_mainEffect->PlayEffect(
		enEffect_Meteo_Explosion,
		position,
		g_vec3One * METEO_EXPLOSION_EFFECT_SIZE,
		Quaternion::Identity,
		false);

	//�󒆂ɂ���Ԃ̉����X�g�b�v
	g_soundManager->StopSound(enSoundName_Boss_Meteo_Fly);

	//���Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_Meteo_Explosion,
		g_soundManager->GetSEVolume()
	);


	DeleteGO(this);
}

void DarkMeteorite::Update()
{
	if (!m_isMove) return;


	//�ړ�
	Move();

	if (IsDelete())
	{
		//����
		Explosion();
		return;
	}

}

void DarkMeteorite::Render(RenderContext& rc)
{
}