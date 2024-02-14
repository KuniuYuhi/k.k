#include "stdafx.h"
#include "ComboFinishBomb.h"
#include "InitEffect.h"

namespace {
	const int ATTACK = 30;
	const float COLLISION_RADIUS = 80.0f;
	const float EFFECT_SIZE = 10.0f;
}

ComboFinishBomb::ComboFinishBomb()
{
}

ComboFinishBomb::~ComboFinishBomb()
{

}

bool ComboFinishBomb::Start()
{
	//攻撃力の設定
	SetAttack(ATTACK);
	//エフェクト再生
	PlayBombEffect();
	//当たり判定作成
	CreatCollision();

	return true;
}

void ComboFinishBomb::Update()
{
	//コリジョンが消えたら消去
	if (m_collision == nullptr)
	{
		DeleteGO(this);
	}
	
}

void ComboFinishBomb::PlayBombEffect()
{
	//爆発エフェクト再生
	EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
	explosionEffect->Init(InitEffect::enEffect_DarkBall_Explosion);
	explosionEffect->Play();
	explosionEffect->SetScale(g_vec3One * EFFECT_SIZE);
	explosionEffect->SetPosition(m_position);
	explosionEffect->Update();

}

void ComboFinishBomb::CreatCollision()
{
	m_collision = NewGO<CollisionObject>(0, GetCollisionName());
	m_collision->CreateSphere(
		m_position,g_quatIdentity, COLLISION_RADIUS
	);
	m_collision->SetCreatorName(GetName());
	//自動で削除するようにする
	m_collision->SetIsEnableAutoDelete(true);
	m_collision->Update();
}
