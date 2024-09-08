#include "stdafx.h"
#include "EyeBall.h"
#include "MagicBallMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"


namespace {
	const int COLLISION_RADIUS = 15.0f;
}

EyeBall::EyeBall()
{
}

EyeBall::~EyeBall()
{
	if (m_collision != nullptr)
	{
		DeleteGO(m_collision);
	}
}

bool EyeBall::Start()
{

	InitModel();

	//�X�e�[�^�X�ݒ�
	m_status.InitMagicBallStatus("EyeBall");

	//�R���|�[�l���g�̐ݒ�
	DefaultSettingComponents();
	AddSettingComponents();

	//�_���[�W����ݒ�
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_status.GetPower(),
		m_status.GetKnockBackTimeScale(),
		m_status.GetKnockBackPattern()
	);

	//�����蔻��쐬
	CreateCollision();



	return true;
}

void EyeBall::AddSettingComponents()
{
	//���@���ړ��R���|�[�l���g��ǉ�
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//�ړ��R���|�[�l���g�Ɏ��g�̃C���X�^���X��ݒ�
	m_magicBallMovement->SetMagicBallInstance(this);


}

void EyeBall::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(
		0,g_collisionObjectManager->m_enemyAttackCollisionName
	);
	//����̓����蔻��
	m_collision->CreateSphere(m_position, m_rotation, COLLISION_RADIUS);

	m_collision->SetCreatorName(GetName());
	//�����ō폜���Ȃ�
	m_collision->SetIsEnableAutoDelete(false);
}

void EyeBall::Move()
{
	if (m_magicBallMovement == nullptr) return;



	//���i
	m_magicBallMovement->MoveStraight();

	m_collision->SetPosition(m_position);
	m_collision->Update();
}

bool EyeBall::IsDelete()
{
	if (IsDeleteTime())
	{
		return true;
	}

	//�����Ƀq�b�g������
	if (m_damageProvider->IsHit())
	{
		return true;
	}



	return false;
}

bool EyeBall::IsDeleteTime()
{
	if (m_deleteTimer >= m_status.GetDeleteTimeLimit())
	{
		//�����\
		return true;
	}
	//�^�C�}�[�����Z
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	return false;
}


void EyeBall::Update()
{
	if (IsDelete())
	{
		//����
		DeleteGO(this);
		return;
	}

	//�ړ�
	Move();

}

void EyeBall::Render(RenderContext& rc)
{
}

void EyeBall::InitModel()
{
}

