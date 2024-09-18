#include "stdafx.h"
#include "RecoveryBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

RecoveryBuffItem::RecoveryBuffItem()
{
	Init();
}

RecoveryBuffItem::~RecoveryBuffItem()
{
}

void RecoveryBuffItem::Start()
{
}

void RecoveryBuffItem::UpdateComponent()
{
	//プレイヤーのHPを回復したら
	if (isRecoveryPlayerHp)
	{
		DeleteProcess();

		delete this;
	}
}

void RecoveryBuffItem::Init()
{
	//ステータスを初期化
	m_status.InitEffectItemStatus("RecoveryHP");

	m_brave = FindGO<Brave>("Brave");

	//プレイヤーのHPを回復させる
	m_brave->GetStatus().RecoveryHp(m_status.GetEffectValue());

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "RecoveryStartEffect");
	m_effect->PlayEffect(
		enEffect_RecoveryStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

	//HPを回復した
	isRecoveryPlayerHp = true;
}

void RecoveryBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//プレイヤーのコンポーネントから自身を削除する
	m_brave->DeleteComponent(this);
}
