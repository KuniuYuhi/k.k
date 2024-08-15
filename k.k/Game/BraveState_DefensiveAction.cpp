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
	m_brave->ActionActive();
}

void BraveState_DefensiveAction::Ubdate()
{


	//回避、防御アクションを終わるなら
	if (m_brave->GetArmedWeapon()->IsEndDefensiveAction())
	{
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}


}

void BraveState_DefensiveAction::Exit()
{
	m_brave->ActionDeactive();
}
