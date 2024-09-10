#include "stdafx.h"
#include "DarkBall.h"
#include "MagicBallMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"

#include "Brave.h"

namespace {
	const int COLLISION_RADIUS = 100.0f;
}

DarkBall::DarkBall()
{
}

DarkBall::~DarkBall()
{
	if (m_collision != nullptr)
	{
		DeleteGO(m_collision);
	}
}

bool DarkBall::Start()
{
	InitModel();

	//�X�e�[�^�X�ݒ�
	m_status.InitMagicBallStatus("DarkBall");

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

	if (m_enShotPatternState == enChase)
	{
		m_player = FindGO<Brave>("Brave");
	}

	return true;
}

void DarkBall::InitModel()
{
}

void DarkBall::AddSettingComponents()
{
	//���@���ړ��R���|�[�l���g��ǉ�
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//�ړ��R���|�[�l���g�Ɏ��g�̃C���X�^���X��ݒ�
	m_magicBallMovement->SetMagicBallInstance(this);
}

void DarkBall::CreateCollision()
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

void DarkBall::Move()
{
	if (m_magicBallMovement == nullptr) return;


	if (m_enShotPatternState == enStraight)
	{
		//���i
		m_magicBallMovement->MoveStraight();
	}
	else if(m_enShotPatternState == enChase)
	{
		m_magicBallMovement->MoveChase(m_player->GetPosition());
	}
	

	m_collision->SetPosition(m_position);
	m_collision->Update();

}

bool DarkBall::IsDelete()
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

bool DarkBall::IsDeleteTime()
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

void DarkBall::Update()
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

void DarkBall::Render(RenderContext& rc)
{
}