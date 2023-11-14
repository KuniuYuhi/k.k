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
	m_movePos = m_position;
	//始点を決める
	m_startPosition = m_position;
	//中間点を決める
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;

	//中間点のY座標を上げる
	m_centerPosition.y = m_startPosition.y + CENTER_POS_ADD_Y;

	m_scale = SCALE;

	m_meteoEffect = NewGO<EffectEmitter>(0);
	m_meteoEffect->Init(InitEffect::enEffect_Meteo);
	m_meteoEffect->Play();
	m_meteoEffect->SetScale(g_vec3One * METEO_EFFECT_SIZE);
	m_meteoEffect->SetPosition(m_position);
	m_meteoEffect->Update();


	//メテオの当たり判定の生成
	/*m_collision = NewGO<CollisionObject>(0, "meteo");
	m_collision->CreateSphere(
		m_position,
		m_rotation,
		METEO_COLLISION_SIZE
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();*/

	//ステート設定
	m_state = enMoveState;

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

	//200ごとにダメージを減らす
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
	//線形補間を使って移動する
	//視点から中点
	StartToCenter.Lerp(m_timer, m_startPosition, m_centerPosition);
	//中点から終点
	CenterToEnd.Lerp(m_timer, m_centerPosition, m_targetPosition);
	//線形補間した２つのベクトルを更に線形補間
	m_movePos.Lerp(m_timer, StartToCenter, CenterToEnd);
	
	m_timer += ADD_TIMER;

	//設定と更新
	//爆発していない間メテオの当たり判定の座標を更新する
	/*m_collision->SetPosition(m_movePos);
	m_collision->Update();*/

	m_meteoEffect->SetPosition(m_movePos);
	m_meteoEffect->Update();
	
	m_position = m_movePos;
}

void Meteo::CreateExplosionCollision()
{
	//爆発範囲の当たり判定作成
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

		//メテオ本体の当たり判定削除
		//DeleteGO(m_collision);

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
