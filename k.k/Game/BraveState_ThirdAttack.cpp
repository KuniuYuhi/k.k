#include "stdafx.h"
#include "BraveState_ThirdAttack.h"
#include "Brave.h"
#include "WeaponBase.h"


void BraveState_ThirdAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ThirdAttack + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_ThirdAttack::Entry()
{
	//武器のエントリーの処理を行う
	m_brave->GetArmedWeapon()->EntryNormalAttackProcess(WeaponBase::enCombo_Third);

	//アクション中にする
	m_brave->ActionActive();
}

void BraveState_ThirdAttack::Ubdate()
{
	//武器の更新処理
	m_brave->GetArmedWeapon()->UpdateNormalAttackProcess(WeaponBase::enCombo_Third);

	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ThirdAttack::Exit()
{
	//武器の抜け出す処理
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_Third);

	//アクションを終わる
	m_brave->ExitAttackAction();
}
