#include "stdafx.h"
#include "RecoveryEffect.h"

#include "UseEffect.h"
#include "Brave.h"

#include "RecoveryBuffItem.h"

namespace {
	const float DELETE_TIME_LIMIT = 20.0f;
	const float HIT_RADIUS = 30.0f;
}

RecoveryEffect::RecoveryEffect()
{
}

RecoveryEffect::~RecoveryEffect()
{
}

bool RecoveryEffect::Start()
{
	Init();

	return true;
}

void RecoveryEffect::Update()
{
	if (IsDelete())
	{
		DeleteProcess();
		return;
	}

	HitPlayer();
}

void RecoveryEffect::Init()
{
	m_brave = FindGO<Brave>("Brave");

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "RecoveryEffectSynbol");
	m_effect->PlayEffect(
		enEffect_RecoveryEffectSynbol,
		m_position,
		g_vec3One * 8.0f,
		Quaternion::Identity,
		false
	);
}

void RecoveryEffect::HitPlayer()
{
	Vector3 diff = m_position - m_brave->GetPosition();
	//プレイヤーにヒットしたら
	if (diff.Length() < HIT_RADIUS)
	{
		//コンポーネントを付与
		m_brave->AddComponent<RecoveryBuffItem>();
		m_isHit = true;
	}
}

bool RecoveryEffect::IsDelete()
{
	if (m_isHit)
	{
		return true;
	}


	if (m_deleteTimer >= DELETE_TIME_LIMIT)
	{
		return true;
	}
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();

	return false;
}

void RecoveryEffect::DeleteProcess()
{
	m_effect->Delete();
	m_effect = nullptr;

	DeleteGO(this);

}
