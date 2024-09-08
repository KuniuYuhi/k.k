#include "stdafx.h"
#include "DarkMeteorite.h"
#include "MagicBallMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"


namespace {
	const int COLLISION_RADIUS = 700.0f;
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
			//衝突したのは壁ではない。
			isHit = false;
			return 0.0f;
		}
		else
		{
			//地面とぶつかったら。
		//フラグをtrueに。
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
}

bool DarkMeteorite::Start()
{
	InitModel();

	//ステータス設定
	m_status.InitMagicBallStatus("DarkMeteorite");

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


void DarkMeteorite::InitModel()
{
}

void DarkMeteorite::AddSettingComponents()
{
	//魔法球移動コンポーネントを追加
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//移動コンポーネントに自身のインスタンスを設定
	m_magicBallMovement->SetMagicBallInstance(this);
}

void DarkMeteorite::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(
		0, g_collisionObjectManager->m_enemyAttackCollisionName
	);
	//球状の当たり判定
	m_collision->CreateSphere(m_position, m_rotation, COLLISION_RADIUS);

	m_collision->SetCreatorName(GetName());
	//自動で削除しない
	m_collision->SetIsEnableAutoDelete(false);
}

void DarkMeteorite::Move()
{
	if (m_magicBallMovement == nullptr) return;

	//落下する
	m_magicBallMovement->MoveFall();

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
	//始点と終点を設定
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(
		btVector3(m_position.x, m_position.y, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(
		btVector3(m_position.x, m_position.y - 300.0f , m_position.z));
	//壁の判定を返す
	IsWallResult isWallResult;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
		start, end, isWallResult);
	//壁にヒットしたか
	if (isWallResult.isHit == true)
	{
		//ヒットした
		return true;
	}
	else
	{
		//ヒットしなかった
		return false;
	}


	return false;
}

void DarkMeteorite::Explosion()
{



	DeleteGO(this);
}

void DarkMeteorite::Update()
{
	if (IsDelete())
	{
		//爆発
		Explosion();
		return;
	}

	//移動
	Move();
}

void DarkMeteorite::Render(RenderContext& rc)
{
}