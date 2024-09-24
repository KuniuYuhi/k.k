#include "stdafx.h"
#include "AttackEffect.h"

#include "UseEffect.h"
#include "Brave.h"

#include "AttackBuffItem.h"

namespace {
	const float DELETE_TIME_LIMIT = 20.0f;
	const float HIT_RADIUS = 60.0f;
}

AttackEffect::AttackEffect()
{
}

AttackEffect::~AttackEffect()
{
}

bool AttackEffect::Start()
{
	Init();

	return true;
}

void AttackEffect::Update()
{
	if (IsDelete())
	{
		DeleteProcess();
		return;
	}


	HitPlayer();
}

void AttackEffect::Init()
{
	
	m_brave = FindGO<Brave>("Brave");

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "AttackEffectSynbol");
	m_effect->PlayEffect(
		enEffect_AttackEffectSynbol,
		m_position,
		g_vec3One * 8.0f,
		Quaternion::Identity,
		false
	);
}

void AttackEffect::HitPlayer()
{
	Vector3 diff = m_position - m_brave->GetPosition();
	//プレイヤーにヒットしたら
	if (diff.Length() < HIT_RADIUS)
	{
		//コンポーネントを付与
		m_brave->AddComponent<AttackBuffItem>();
		m_isHit = true;
	}
}

bool AttackEffect::IsDelete()
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

void AttackEffect::DeleteProcess()
{

	m_effect->Delete();
	m_effect = nullptr;

	DeleteGO(this);
}
