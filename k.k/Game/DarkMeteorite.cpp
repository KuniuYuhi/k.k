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


	m_areaEffect = nullptr;
	m_mainEffect = nullptr;
}

bool DarkMeteorite::Start()
{
	InitModel();

	//ステータス設定
	m_status.InitMagicBallStatus("DarkMeteorite");

	//コンポーネントの設定
	DefaultSettingComponents();
	AddSettingComponents();

	//音再生
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
	//ダメージ情報を設定
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_status.GetPower(),
		m_status.GetKnockBackTimeScale(),
		m_status.GetKnockBackPattern()
	);

	//当たり判定作成
	CreateCollision();

	//エフェクトを生成
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
	//エフェクト生成
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

	Vector3 effectPosition = m_position;

	effectPosition.y -= ADD_Y_DOWN;

	//エフェクト座標を設定
	m_mainEffect->SetMovePosition(effectPosition);

	//当たり判定の座標更新
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
		btVector3(m_position.x, m_position.y - END_Y_ADD_VALUE, m_position.z));
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
	m_areaEffect->Delete();
	m_areaEffect = nullptr;

	m_mainEffect->Delete();
	m_mainEffect = nullptr;

	Vector3 position = m_position;
	position.y = 0.0f;

	m_mainEffect = NewGO<UseEffect>(0, "DarkMeteoriteExplosionEffect");
	//爆発エフェクト
	m_mainEffect->PlayEffect(
		enEffect_Meteo_Explosion,
		position,
		g_vec3One * METEO_EXPLOSION_EFFECT_SIZE,
		Quaternion::Identity,
		false);

	//空中にいる間の音をストップ
	g_soundManager->StopSound(enSoundName_Boss_Meteo_Fly);

	//音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_Meteo_Explosion,
		g_soundManager->GetSEVolume()
	);


	DeleteGO(this);
}

void DarkMeteorite::Update()
{
	if (!m_isMove) return;


	//移動
	Move();

	if (IsDelete())
	{
		//爆発
		Explosion();
		return;
	}

}

void DarkMeteorite::Render(RenderContext& rc)
{
}