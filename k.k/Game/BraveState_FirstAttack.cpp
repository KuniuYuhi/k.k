#include "stdafx.h"
#include "BraveState_FirstAttack.h"
#include "Brave.h"
#include "PlayerController.h"
#include "WeaponBase.h"

void BraveState_FirstAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_FirstAttack + m_brave->GetCurrentAnimationStartIndexNo(),0.1f
	);
}

void BraveState_FirstAttack::Entry()
{
	if (m_playerController == nullptr)
	{
		//プレイヤーコントローラーコンポーネントを取得
		m_playerController = m_brave->GetComponent<PlayerController>();
	}

	//武器のエントリーの処理を行う
	m_brave->GetArmedWeapon()->EntryNormalAttackProcess(WeaponBase::enCombo_First);

	//アクション中にする
	m_brave->ActionActive();
	//トリガーチェックフラグをリセット
	m_isTrigger = false;
	//
	m_isNextNormalAttackProgress = false;
}

void BraveState_FirstAttack::Ubdate()
{
	//次の通常攻撃待機区間内で通常攻撃ボタンを押したなら
	if (m_playerController->IsTriggerNromalAttackButton()&& !m_isTrigger&&
		m_brave->GetArmedWeapon()->IsStandbyPeriod())
	{
		m_isTrigger = true;
	}

	//武器の更新処理
	m_brave->GetArmedWeapon()->UpdateNormalAttackProcess(WeaponBase::enCombo_First);

	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		
		//ボタンを押していたら
		if (m_isTrigger)
		{
			m_isNextNormalAttackProgress = true;
			//次のコンボ処理をする
			m_brave->NormalAttackProcess();
			
		}
		else
		{
			//ステートの共通処理
			m_brave->ProcessCommonStateTransition();
		}
		
	}

}

void BraveState_FirstAttack::Exit()
{
	//次の攻撃に移らないなら
	//todo 被ダメージを受けていてもやる
	if (!m_isNextNormalAttackProgress)
	{
		//アクションを終わる
		m_brave->ExitAttackAction();
	}
	
	//武器の抜け出す処理
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_First);

	//トリガーチェックフラグをリセット
	m_isTrigger = false;
	
}
