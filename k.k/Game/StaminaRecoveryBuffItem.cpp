#include "stdafx.h"
#include "StaminaRecoveryBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

StaminaRecoveryBuffItem::StaminaRecoveryBuffItem()
{
	Init();
}

StaminaRecoveryBuffItem::~StaminaRecoveryBuffItem()
{
}

void StaminaRecoveryBuffItem::Start()
{
}

void StaminaRecoveryBuffItem::UpdateComponent()
{
	//プレイヤーのスタミナを回復したら
	if (isRecoveryPlayerStamina)
	{
		DeleteProcess();

		delete this;
	}
}

void StaminaRecoveryBuffItem::Init()
{
	//ステータスを初期化
	m_status.InitEffectItemStatus("RecoveryStamina");

	m_brave = FindGO<Brave>("Brave");

	//プレイヤーのスタミナを回復させる
	m_brave->GetStatus().RecoveryStamina(m_status.GetEffectValue());

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "RecoveryStaminaStartEffect");
	m_effect->PlayEffect(
		enEffect_StaminaRecoveryStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

	//効果音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_ItemStaminaRecovery,
		g_soundManager->GetSEVolume()
	);

	//スタミナを回復した
	isRecoveryPlayerStamina = true;
}

void StaminaRecoveryBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//プレイヤーのコンポーネントから自身を削除する
	m_brave->DeleteComponent(this);
}

