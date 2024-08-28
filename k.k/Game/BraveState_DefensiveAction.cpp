#include "stdafx.h"
#include "BraveState_DefensiveAction.h"
#include "Brave.h"
#include "WeaponBase.h"

void BraveState_DefensiveAction::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_DefensiveActions + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_DefensiveAction::Entry()
{
	//アクション中にする
	m_brave->ActionActive();

	//装備している武器の回避、防御アクションに入った時の最初の処理を実行
	m_brave->GetArmedWeapon()->EntryDefensiveActionProcess();
}

void BraveState_DefensiveAction::Ubdate()
{
	//装備している武器の更新処理
	m_brave->GetArmedWeapon()->UpdateDefensiveActionProcess();

	//回避、防御アクションを終わるなら
	//if (m_brave->GetArmedWeapon()->IsEndDefensiveAction())
	//{
	//	//ステートの共通処理
	//	m_brave->ProcessCommonStateTransition();
	//}


}

void BraveState_DefensiveAction::Exit()
{
	m_brave->GetArmedWeapon()->ExitDefensiveActionProcess();

	m_brave->ActionDeactive();
}
