#include "stdafx.h"
#include "DarkBall.h"
#include "MagicBallMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"

#include "Brave.h"

#include "UseEffect.h"

namespace {
	const float COLLISION_RADIUS = 60.0f;

	const float MOVE_Y_UP = 110.0f;

	const float ADD_RADIUS = 80.0f;

	const float MUL_DARKBALL_EFFECT_SIZE = 20.0f;
	const float MUL_DARKBALL_EXPLOSION_EFFECT_SIZE = 10.0f;
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

	m_effect = nullptr;
}

bool DarkBall::Start()
{
	InitModel();

	//ステータス設定
	m_status.InitMagicBallStatus("DarkBall");

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

	m_effect = NewGO<UseEffect>(0, "darkBallEffect");
	m_effect->PlayEffect(
		enEffect_DarkBall,
		m_position,
		g_vec3One * MUL_DARKBALL_EFFECT_SIZE,
		Quaternion::Identity,
		false
	);

	//ショットパターンが追いかけるなら
	if (m_enShotPatternState == enChase)
	{
		m_player = FindGO<Brave>("Brave");

		m_status.SetDeleteTimeLimit(m_status.GetDeleteTimeLimit() * 2.0f);
	}

	return true;
}

void DarkBall::InitModel()
{
}

void DarkBall::AddSettingComponents()
{
	//魔法球移動コンポーネントを追加
	AddComponent<MagicBallMovement>();
	m_magicBallMovement = GetComponent<MagicBallMovement>();
	//移動コンポーネントに自身のインスタンスを設定
	m_magicBallMovement->SetMagicBallInstance(this);
}

void DarkBall::CreateCollision()
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

void DarkBall::Move()
{
	if (m_magicBallMovement == nullptr) return;


	if (m_enShotPatternState == enStraight)
	{
		//直進
		m_magicBallMovement->MoveStraight(MOVE_Y_UP,true);
	}
	else if(m_enShotPatternState == enChase)
	{
		m_magicBallMovement->MoveChase(
			m_player->GetPosition(), COLLISION_RADIUS + ADD_RADIUS,
			MOVE_Y_UP, true);
	}
	
	//エフェクトの移動
	m_effect->SetMovePosition(m_position);

	m_collision->SetPosition(m_position);
	m_collision->Update();

}

bool DarkBall::IsDelete()
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

bool DarkBall::IsDeleteTime()
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

void DarkBall::Explosion()
{
	//今のエフェクトを削除
	m_effect->Delete();

	m_effect = nullptr;
	//新たに爆発エフェクトを生成
	m_effect = NewGO<UseEffect>(0, "darkBallExplosionEffect");
	m_effect->PlayEffect(
		enEffect_DarkBall_Explosion,
		m_position,
		g_vec3One * MUL_DARKBALL_EXPLOSION_EFFECT_SIZE,
		Quaternion::Identity,
		false
	);

	//音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_DarkBall_Explosion,
		g_soundManager->GetSEVolume()
	);

	//自身は消去
	DeleteGO(this);
}

void DarkBall::Update()
{
	//移動
	Move();

	if (IsDelete())
	{
		Explosion();
		return;
	}
}

void DarkBall::Render(RenderContext& rc)
{
}