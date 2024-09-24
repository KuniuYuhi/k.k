#include "stdafx.h"
#include "StaminaEffect.h"

#include "StaminaRecoveryBuffItem.h"

#include "UseEffect.h"
#include "Brave.h"



namespace {
	const float DELETE_TIME_LIMIT = 30.0f;
	const float HIT_RADIUS = 60.0f;
}

StaminaEffect::StaminaEffect()
{
}

StaminaEffect::~StaminaEffect()
{
}

bool StaminaEffect::Start()
{
	Init();

	return true;
}

void StaminaEffect::Update()
{
	if (IsDelete())
	{
		DeleteProcess();
		return;
	}


	HitPlayer();
}

void StaminaEffect::Init()
{
	m_brave = FindGO<Brave>("Brave");

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "StaminaRecoveryEffectSynbol");
	m_effect->PlayEffect(
		enEffect_StaminaRecoveryEffectSynbol,
		m_position,
		g_vec3One * 8.0f,
		Quaternion::Identity,
		false
	);
}

void StaminaEffect::HitPlayer()
{
	Vector3 diff = m_position - m_brave->GetPosition();
	//プレイヤーにヒットしたら
	if (diff.Length() < HIT_RADIUS)
	{
		//コンポーネントを付与
		m_brave->AddComponent<StaminaRecoveryBuffItem>();
		m_isHit = true;
	}
}

bool StaminaEffect::IsDelete()
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

void StaminaEffect::DeleteProcess()
{
	m_effect->Delete();
	m_effect = nullptr;

	DeleteGO(this);
}
