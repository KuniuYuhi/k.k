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

//衝突したときに呼ばれる関数オブジェクト(地面用)
struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
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

bool Meteo::Start()
{
	//攻撃力の設定
	SetAttack(ATTACK);

	m_scale = SCALE;

	SettingMeteoRoute();

	//メテオエフェクト再生
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
	//ベクトルを求める
	Vector3 diff = targetPosition - m_position;
	diff.y = 0.0f;
	float length = diff.Length();

	//距離が200ごとにダメージを減らす
	float reduce = length / DIV_DISTANCE;

	int attack = m_attack; 
	attack -= reduce * 10.0f;

	//最低でも20ダメージになるようにする
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
	//	//地面についた
	//	//爆発用当たり判定の生成
	//	CreateExplosionCollision();
	//	//爆発した
	//	SetExplosionFlag(true);

	//	//爆発エフェクトの生成
	//	while (m_ExplosionEffect == nullptr)
	//	{
	//		//爆発する
	//		m_explosionEffectFlag = true;
	//		Explosion();
	//	}

	//	//次のステートに遷移
	//	m_state = enExplosionState;
	//	return;
	//}
	

	//線形補間を使って移動する
	//視点から中点
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//中点から終点
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//線形補間した２つのベクトルを更に線形補間
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);
	
	m_timer += ADD_TIMER;

	//設定と更新
	m_meteoEffect->SetPosition(m_movePos);
	m_meteoEffect->Update();
	
	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//爆発範囲の当たり判定作成
	auto explosionCollision = NewGO<CollisionObject>(0, GetCollisionName());
	explosionCollision->CreateSphere(
		m_position,
		g_quatIdentity,
		EXPLOSION_COLLISION_SIZE
	);
	explosionCollision->SetPosition(m_position);
	//当たり判定を生成した作成者の設定
	explosionCollision->SetCreatorName(GetName());
	//自動で削除するようにする
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
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(m_position.x, m_position.y + downValue, m_position.z));
	//壁の判定を返す
	IsGroundResult callback_Ground;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
		start, end, callback_Ground);
	//地面に衝突した
	if (callback_Ground.isHit == true)
	{
		return true;
	}
	else
	{
		//地面ではなかった
		return false;
	}
}

void Meteo::Explosion()
{
	//エフェクト生成
	m_ExplosionEffect = NewGO<EffectEmitter>(0);
	m_ExplosionEffect->Init(InitEffect::enEffect_Meteo_Explosion);
	m_ExplosionEffect->Play();
	m_ExplosionEffect->SetPosition(m_position);
	m_ExplosionEffect->Update();
}

const int Meteo::GetAttack()
{
	//プレイヤーの座標を取得
	Vector3 playerPos = FindGO<Player>("player")->GetPosition();
	//プレイヤーの座標によってダメージ変更
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
	//始点を決める
	m_startPosition = m_position;
	//中間点を決める
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;

	//中間点のY座標を上げる
	m_centerPosition.y = m_startPosition.y + CENTER_POS_ADD_Y;

	//1.目標に向かう距離の計算
	//終点-始点
	//Vector3 targetDistance = m_targetPosition - m_startPosition;
	////前方向を設定
	//m_forward = targetDistance;
	//m_forward.Normalize();
	//float distance = targetDistance.Length();
	////メテオが撃ちあがる角度を決める
	//Vector3 toAngle = targetDistance;
	//toAngle.Normalize();
	//toAngle.Length();
	//m_angle = Math::Lerp(toAngle.Length(), 60.0f, 30.0f);

	////2.初速度の計算
	//float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	////3.初速度の分解
	//m_meteoVerocity.x = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	//m_meteoVerocity.y = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	////4.飛行時間の計算
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
	//地面に着いたら
	if (IsGroundCheck(DOWN) == true || m_movePos.y <= EXPLOSION_Y_POS)
	{
		//地面についた
		//爆発用当たり判定の生成
		CreateExplosionCollision();
		//爆発した
		SetExplosionFlag(true);

		//爆発エフェクトの生成
		while (m_ExplosionEffect == nullptr)
		{
			//爆発する
			m_explosionEffectFlag = true;
			Explosion();
		}

		//次のステートに遷移
		m_state = enExplosionState;
		return;
	}
	//移動処理
	Move();
}

void Meteo::OnProcessExplosionTransition()
{
	//爆発してからの処理
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
