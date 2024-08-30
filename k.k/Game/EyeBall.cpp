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

	//ステータス設定
	m_status.InitMagicBallStatus("EyeBall");

	//コンポーネントの設定
	DefaultSettingComponents();
	AddSettingComponents();

	//ダメージ情報を設定
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_status.GetPower(),
		m_status.GetKnockBackTimeScale(),
		m_status.GetKnockBackPattern()
	);

	//当たり判定作成
	CreateCollision();



	return true;
}

void EyeBall::AddSettingComponents()
{
	//魔法球移動コンポーネントを追加
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//移動コンポーネントに自身のインスタンスを設定
	m_magicBallMovement->SetMagicBallInstance(this);


}

void EyeBall::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(
		0,g_collisionObjectManager->m_enemyAttackCollisionName
	);
	//球状の当たり判定
	m_collision->CreateSphere(m_position, m_rotation, COLLISION_RADIUS);

	m_collision->SetCreatorName(GetName());
	//自動で削除しない
	m_collision->SetIsEnableAutoDelete(false);
}

void EyeBall::Move()
{
	if (m_magicBallMovement == nullptr) return;



	//直進
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

	//何かにヒットしたら
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
		//消去可能
		return true;
	}
	//タイマーを加算
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	return false;
}


void EyeBall::Update()
{
	if (IsDelete())
	{
		//消去
		DeleteGO(this);
		return;
	}

	//移動
	Move();

}

void EyeBall::Render(RenderContext& rc)
{
}

void EyeBall::InitModel()
{
}

