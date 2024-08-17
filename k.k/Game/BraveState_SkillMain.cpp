#include "stdafx.h"
#include "BraveState_SkillMain.h"
#include "Brave.h"
#include "WeaponBase.h"

void BraveState_SkillMain::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Skill_Main + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_SkillMain::Entry()
{
	//アクション中にする
	m_brave->ActionActive();
	//
	m_brave->GetArmedWeapon()->EntrySkillAttackProcess(WeaponBase::enMain);

}

void BraveState_SkillMain::Ubdate()
{
	m_brave->GetArmedWeapon()->UpdateSkillAttackProcess(WeaponBase::enMain);

	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_SkillMain::Exit()
{
	//アクションを終わる
	m_brave->ActionDeactive();

	m_brave->GetArmedWeapon()->ExitSkillAttackProcess(WeaponBase::enMain);
}
