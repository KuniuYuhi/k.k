#include "stdafx.h"
#include "Meteo.h"
#include "InitEffect.h"

//todo メテオが爆発するとき処理が止まる

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
	//m_model.Init("Assets/modelData/character/Lich/Effect/Meteo.tkm", nullptr, 0, enModelUpAxisZ, false, false, false);

	
	m_movePos = m_position;
	//始点を決める
	m_startPosition = m_position;
	//中間点を決める
	m_centerPosition = (m_targetPosition + m_position) / 2.0f;
	//Y座標を上げる
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


	//メテオの当たり判定の生成
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
	//爆発してからの処理
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
			//爆発する
			m_explosionEffectFlag = true;
			//爆発エフェクトの生成
			while (m_ExplosionEffect == nullptr)
			{
				Explosion();
			}
			
		}
	}
	
	//爆発するまでの処理
	//地面につくまで
	if(IsGroundCheck(DOWN)==true || m_movePos.y <= 0.0f)
	{
		//爆発用当たり判定の生成
		CreateExplosionCollision();
		//爆発した
		SetExplosionFlag(true);
	}
	else
	{
		Move();
	}
}

int Meteo::CalcDamageToDistance(const Vector3 targetPosition)
{
	//ベクトルを求める
	Vector3 diff = targetPosition - m_position;
	diff.y = 0.0f;
	float length = diff.Length();

	//100ごとにダメージを減らす
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
	//
	m_timer += 0.01f*0.65f;

	//設定と更新
	//爆発していない間メテオの当たり判定の座標を更新する
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
	//爆発範囲の当たり判定作成
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
