#include "stdafx.h"
#include "AttackBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

AttackBuffItem::AttackBuffItem()
{
	Init();
}

AttackBuffItem::~AttackBuffItem()
{
}

void AttackBuffItem::Start()
{
	
}

void AttackBuffItem::UpdateComponent()
{
	//制限時間になったら自動的に削除する
	if (IsDelete())
	{
		DeleteProcess();

		delete this;
	}

}

void AttackBuffItem::Init()
{
	//ステータスを初期化
	m_status.InitEffectItemStatus("AttackBuff");

	m_brave = FindGO<Brave>("Brave");

	//プレイヤーの攻撃力を増加させる
	m_brave->GetStatus().AddPowerToDefaultPower(m_status.GetEffectValue());
	//攻撃バフを手に入れたのでカウントを増やす
	m_brave->CalcAttackBuffCount(true);

	//エフェクト生成
	m_effect = NewGO<UseEffect>(0, "AttackBuffEffect");
	m_effect->PlayEffect(
		enEffect_AttackUpStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

}

void AttackBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//プレイヤーの攻撃力を増加させた分減少させる
	m_brave->GetStatus().AddPowerToDefaultPower(-m_status.GetEffectValue());

	//攻撃バフが終わるのでカウントを減らす
	m_brave->CalcAttackBuffCount(false);

	//プレイヤーのコンポーネントから自身を削除する
	m_brave->DeleteComponent(this);
}

bool AttackBuffItem::IsDelete()
{
	if (m_deleteTimer >= m_status.GetDuration())
	{
		return true;
	}
	//タイマーを加算
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();

	return false;
}
