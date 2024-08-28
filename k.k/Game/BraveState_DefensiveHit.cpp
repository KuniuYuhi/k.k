#include "stdafx.h"
#include "BraveState_DefensiveHit.h"
#include "Brave.h"
#include "BraveStateContext.h"
#include "WeaponBase.h"

void BraveState_DefensiveHit::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_DefensiveHit + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_DefensiveHit::Entry()
{
	//
	m_brave->EntryHitActionProcess();
	//武器の開始処理
	m_brave->GetArmedWeapon()->EntryDefensiveHitProcess();
}

void BraveState_DefensiveHit::Ubdate()
{
	m_brave->GetArmedWeapon()->UpdateDefensiveHitProcess();
	//アニメーションが終わったら
	m_brave->UpdateHitActionProcess();
}

void BraveState_DefensiveHit::Exit()
{
	//もう一度盾で守るかもしれないので武器の処理が先
	m_brave->GetArmedWeapon()->ExitDefensiveHitProcess();
	//ヒットアクションを抜けるときの処理
	m_brave->ExitHitActionProcess();
}
